#ifndef __METRICS_AND_DATAS__
#define __METRICS_AND_DATAS__

enum class Coarse_Cells_Type : uint32_t {
  ISOTROPIC = 1 << 0,
  ANISOTROPIC = 1 << 1,
  ALL = ISOTROPIC | ANISOTROPIC
};

constexpr enum Coarse_Cells_Type operator |( const enum Coarse_Cells_Type selfValue, const enum Coarse_Cells_Type inValue )
{
    return (enum Coarse_Cells_Type)(uint32_t(selfValue) | uint32_t(inValue));
}

enum class Available_Characteristic : uint32_t {
  NONE = 0,
  MESH = 1 << 0, 
  ASPECT_RATIO = 1 << 1,
  CARD = 1 << 2,
  CARD_DIST = 1 << 3,
  COMPACTNESS = 1 << 4,
  COMPACTNESS_DIST = 1 << 5,
  ISOTROPIC = CARD | CARD_DIST | COMPACTNESS | COMPACTNESS_DIST, 
  DIST = CARD_DIST | COMPACTNESS_DIST,
  ALL = MESH | ASPECT_RATIO | CARD | CARD_DIST | COMPACTNESS | COMPACTNESS_DIST  
};

constexpr enum Available_Characteristic operator |( const enum Available_Characteristic selfValue, const enum Available_Characteristic inValue )
{
    return (enum Available_Characteristic)(uint32_t(selfValue) | uint32_t(inValue));
}

constexpr enum Available_Characteristic operator &( const enum Available_Characteristic selfValue, const enum Available_Characteristic inValue )
{
    return (enum Available_Characteristic)(uint32_t(selfValue) & uint32_t(inValue));
}

enum class Available_Datas : uint32_t {
  AGGLOMERATION_LINES = 1 << 0,
  CELL_INDEX = 1 << 1,
  COMPLIANT_ANISOTROPIC_CELLS = 1 << 2,
  CELLS_ON_BND = 1 << 3,
  CELL_COLOR = 1 << 4,
  ALL_DATAS = AGGLOMERATION_LINES | CELL_INDEX | CELL_COLOR | CELLS_ON_BND,

  CARD = 1 << 5,
  COMPACTNESS = 1 << 6,
  
  ALL_TOPOLOGICAL_METRICS = CARD | COMPACTNESS,
  
  ASPECT_RATIO = 1 << 7, 
  ALL_GEOMETRIC_METRICS = ASPECT_RATIO,
  ALL_METRICS = ALL_TOPOLOGICAL_METRICS | ALL_GEOMETRIC_METRICS,

  ALL = ALL_DATAS | ALL_METRICS
};

constexpr enum Available_Datas operator |( const enum Available_Datas selfValue, const enum Available_Datas inValue )
{
    return (enum Available_Datas)(uint32_t(selfValue) | uint32_t(inValue));
}

#endif
