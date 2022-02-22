import abc 
import numpy as np
from numba import njit, prange

class Face(abc.ABC):
    def __init__(self,points):
        self.points = points
    @property
    def area(self):
        return self._area

    @area.setter
    def area(self, new_area):
        if (new_area>0):
            self._area = new_area
        else:
            print("Error: the area of this solid is 0")
    @area.deleter
    def area(self):
        del self._area
    @property
    def n_vertices(self):
        return self._n_vertices
    @n_vertices.setter
    def n_vertices(self, new_n_vertices):
        if (new_n_vertices > 0):
           self._n_vertices = new_n_vertices
        else:
           print("Error: the n_segments of a face is always > 0")
    @n_vertices.deleter
    def n_vertices(self):
        del self._n_vertices
    @abc.abstractmethod
    def ComputeArea(self):
        """ compute the Area of the faces of the cell with respect to the dimensionality """
        raise NotImplementedError

class Face2D(Face) :
    def __init__(self, segments,*args, **kwargs):
        self.segments = segments
        self.leng_segments = []
        super().__init__(*args, **kwargs)
        self.n_vertices = int(len(self.points)/2)
     
    @staticmethod
    @njit
    def algebric_area(points,shifted): 
        # TODO look if we should shift all to the father: alias to have a global method ComputeArea and re-structure the concept of interface base-class to a simple abstract class. The static method has been implemented to try to use Numba.
        area_0 = 0.5 * np.sum((shifted[:, 0] + points[:, 0])*(shifted[:, 1] - points[:, 1]))
        return area_0

    def ComputeArea(self):
        cell_points_reshaped=np.reshape(self.points,(self.n_vertices,2))
        shifted = np.roll(cell_points_reshaped, 1, axis=0)
        area_0 = self.algebric_area(cell_points_reshaped,shifted)
        self.area = abs(area_0)
                  
    def ComputeLength(self,global_points):
        for segment in self.segments:
             leng = np.sqrt((global_points[segment[1]][1]-global_points[segment[0]][1])**2+(global_points[segment[1]][0]-global_points[segment[0]][0])**2)
             self.leng_segments.append(leng)

class Face3D(Face) :
    def __init__(self,*args, **kwargs):
        super().__init__(*args, **kwargs)
        self.n_vertices = int(len(self.points)/3)
        self.points_reshaped = np.reshape(self.points,(self.n_vertices,3))  
        self.a = self.points_reshaped[0]
        self.b = self.points_reshaped[1]
        self.c = self.points_reshaped[2]

    def unit_normal(self):
        x = np.linalg.det([[1,self.a[1],self.a[2]],
             [1,self.b[1],self.b[2]],
             [1,self.c[1],self.c[2]]])
        y = np.linalg.det([[self.a[0],1,self.a[2]],
             [self.b[0],1,self.b[2]],
             [self.c[0],1,self.c[2]]])
        z = np.linalg.det([[self.a[0],self.a[1],1],
             [self.b[0],self.b[1],1],
             [self.c[0],self.c[1],1]])
        magnitude = (x**2 + y**2 + z**2)**.5
        return ([x/magnitude, y/magnitude, z/magnitude])

    def ComputeArea(self):
        #https://stackoverflow.com/questions/12642256/find-area-of-polygon-from-xyz-coordinates
        normal = self.unit_normal()
        rolled = np.roll(self.points_reshaped,1,axis=0)
      #  total = np.array([])
        total = np.zeros((1,3))
        for i in range(len(self.points_reshaped)):
            product = np.cross(self.points_reshaped[i],rolled[i])
            total = np.add(total,product)
        area_0 = np.dot(total,normal)
        self.area = abs(area_0/2)


class Solid(abc.ABC):
    """ Interface class to compute the different characteristics of an element of
#        a 3D mesh """
    def __init__(self,points,Faces):
        self.Faces = Faces
        self.points = points
    # we can think to set it as a property
    # look at https://stackoverflow.com/questions/37564798/python-property-on-a-list
        self.AreaFaces = [] 
        self.n_faces = int(len(self.Faces))
    @property
    def n_vertices(self):
        return self._n_vertices
    @n_vertices.setter
    def n_vertices(self, new_n_vertices):
        if (new_n_vertices > 0):
           self._n_vertices = new_n_vertices
        else:
           print("Error: the n_segments of a face is always > 0")
    @n_vertices.deleter
    def n_vertices(self):
        del self._n_vertices

    @property
    def volume(self):
        return self._volume

    @volume.setter
    def volume(self, new_volume):
        if new_volume > 0:
            self._volume = new_volume
        else:
            print("Error: the volume of this solid is 0")
    @volume.deleter
    def volume(self):
        del self._volume
    @property
    def n_faces(self):
        return self._n_faces
    @n_faces.setter
    def n_faces(self, new_n_faces):
        if (new_n_faces >= 0):
           self._n_vertices = new_n_faces
        else:
           print("Error: the n_faces of a face is always > 0")
    @n_faces.deleter
    def n_faces(self):
        del self._n_faces

    @abc.abstractmethod
    def ComputeArea(self,globalPoints):
        """ compute the Area of the faces of the cell with respect to the dimensionality """
        raise NotImplementedError

    @abc.abstractmethod
    def ComputeVolume(self):
        """ compute the Volume of the faces of the cell with respect to the dimensionality """
        raise NotImplementedError


class Tetra(Solid):
    def __init__(self,*args, **kwargs):
        super().__init__(*args, **kwargs)
        self.n_vertices = int(len(self.points)/3)
        assert(self.n_vertices==4)
    def ComputeArea(self,global_points):
        """ compute the Area of the faces of the cell with respect to the dimensionality """
        if self.Faces:
         points = []
         for faccia in self.Faces:
             for i,e in enumerate(faccia):
                 points.extend(global_points[e])
             faccia_el = Face3D(points)
             faccia_el.ComputeArea() 
             self.AreaFaces.append(faccia_el.area)
             points = []
    def ComputeVolume(self):
        """ compute the Volume of  of the cells with respect to the dimensionality """
        # https://stackoverflow.com/questions/9866452/calculate-volume-of-any-tetrahedron-given-4-points
        mat = np.array(self.points)
        mat_1=np.reshape(mat,(self.n_vertices,3)).transpose()
        mat_2=  np.vstack([mat_1,np.ones((1,4))])
        self.volume= 1/6*abs(np.linalg.det(mat_2))
