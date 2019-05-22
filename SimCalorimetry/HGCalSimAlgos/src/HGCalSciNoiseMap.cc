#include "SimCalorimetry/HGCalSimAlgos/interface/HGCalSciNoiseMap.h"

//
HGCalSciNoiseMap::HGCalSciNoiseMap() :
  refEdge_(3.)
{
}

//
std::pair<double, double> HGCalSciNoiseMap::scaleByDose(const HGCScintillatorDetId& cellId,  const std::array<double, 8>& radius)
{
  if(getDoseMap().empty())
    return std::make_pair(1., 0.);

  int layer = cellId.layer();
  double cellDose = getDoseValue(DetId::HGCalHSc, layer, radius); //in kRad
  constexpr double expofactor = 1./199.6;
  double scaleFactor = std::exp( -std::pow(cellDose, 0.65) * expofactor );

  double cellFluence = getFluenceValue(DetId::HGCalHSc, layer, radius); //in 1-Mev-equivalent neutrons per cm2

  constexpr double factor = 2. / (2*1e13); //SiPM area = 2mm^2
  double noise = 2.18 * sqrt(cellFluence * factor);


  return std::make_pair(scaleFactor, noise);
}

double HGCalSciNoiseMap::scaleByArea(const HGCScintillatorDetId& cellId, const std::array<double, 8>& radius)
{
  double edge;
  if(cellId.type() == 0)
  {
    constexpr double factor = 2 * M_PI * 1./360.;
    edge = radius[0] * factor; //1 degree
  }
  else
  {
    constexpr double factor = 2 * M_PI * 1./288.;
    edge = radius[0] * factor; //1.25 degrees
  }

  double scaleFactor = refEdge_ / edge;  //assume reference 3cm of edge


  return scaleFactor;
}

std::array<double, 8> HGCalSciNoiseMap::computeRadius(const HGCScintillatorDetId& cellId)
{
  GlobalPoint global = geom()->getPosition(cellId);

  double radius2 = std::pow(global.x(), 2) + std::pow(global.y(), 2); //in cm
  double radius4 = std::pow(radius2, 2);
  double radius = sqrt(radius2);
  double radius3 = radius2*radius2;

  double radius_m100 = radius-100;
  double radius_m100_2 = std::pow(radius_m100, 2);
  double radius_m100_3 = radius_m100_2*radius_m100;
  double radius_m100_4 = std::pow(radius_m100_2, 2);

  std::array<double, 8> radii { {radius, radius2, radius3, radius4, radius_m100, radius_m100_2, radius_m100_3, radius_m100_4} };
  return radii;
}
