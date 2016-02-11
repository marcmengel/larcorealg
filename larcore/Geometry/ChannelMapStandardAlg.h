////////////////////////////////////////////////////////////////////////
/// \file  ChannelMapStandardAlg.h
/// \brief Interface to algorithm class for a specific detector channel mapping
///
/// \version $Id:  $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#ifndef GEO_CHANNELSTANDARDMAPALG_H
#define GEO_CHANNELSTANDARDMAPALG_H

#include <vector>
#include <set>
#include <iostream>

#include "larcore/SimpleTypesAndConstants/geo_types.h"
#include "larcore/Geometry/ChannelMapAlg.h"
#include "larcore/Geometry/GeoObjectSorterStandard.h"
#include "fhiclcpp/ParameterSet.h"

namespace geo{

  class ChannelMapStandardAlg : public ChannelMapAlg{

  public:

    ChannelMapStandardAlg(fhicl::ParameterSet const& p);
    
    void                     Initialize( GeometryData_t& geodata ) override;
    void                     Uninitialize();
    std::vector<WireID>      ChannelToWire(raw::ChannelID_t channel)     const;
    unsigned int             Nchannels()                                 const;

    //@{
    virtual double WireCoordinate
      (double YPos, double ZPos, geo::PlaneID const& planeID) const override;
    virtual double WireCoordinate(double YPos, double ZPos,
                                 unsigned int PlaneNo,
                                 unsigned int TPCNo,
                                 unsigned int cstat) const
      { return WireCoordinate(YPos, ZPos, geo::PlaneID(cstat, TPCNo, PlaneNo)); }
    //@}
    
    //@{
    virtual WireID NearestWireID
      (const TVector3& worldPos, geo::PlaneID const& planeID) const override;
    virtual WireID NearestWireID(const TVector3& worldPos,
                                 unsigned int    PlaneNo,
                                 unsigned int    TPCNo,
                                 unsigned int    cstat) const override
      { return NearestWireID(worldPos, geo::PlaneID(cstat, TPCNo, PlaneNo)); }
    //@}
    
    //@{
    virtual raw::ChannelID_t PlaneWireToChannel
      (geo::WireID const& wireID) const override;
    virtual raw::ChannelID_t PlaneWireToChannel(unsigned int plane,
                                                unsigned int wire,
                                                unsigned int tpc,
                                                unsigned int cstat) const override
      { return PlaneWireToChannel(geo::WireID(cstat, tpc, plane, wire)); }
    //@}
    
    virtual View_t                   View( raw::ChannelID_t const channel )       const override;
    virtual SigType_t                SignalType( raw::ChannelID_t const channel ) const override;
    virtual std::set<View_t>  const& Views()                                      const override;
    virtual std::set<PlaneID> const& PlaneIDs()                                   const override;
  
  private:
    
    unsigned int                                         fNcryostat;      ///< number of cryostats in the detector
    unsigned int                                         fNchannels;      ///< number of channels in the detector
    raw::ChannelID_t                                     fTopChannel;     ///< book keeping highest channel #
    std::vector<unsigned int>                            fNTPC;           ///< number of TPCs in each cryostat
    std::set<View_t>                                     fViews;          ///< vector of the views present in the detector
    std::set<PlaneID>                                    fPlaneIDs;       ///< vector of the PlaneIDs present in the detector
    PlaneInfoMap_t<float>                                fFirstWireProj;  ///< Distance (0,0,0) to first wire          
                                                                          ///< along orth vector per plane per TPC
    PlaneInfoMap_t<float>                                fOrthVectorsY;   ///< Unit vectors orthogonal to wires in
    PlaneInfoMap_t<float>                                fOrthVectorsZ;   ///< each plane - stored as 2 components
                                                                          ///< to avoid having to invoke any bulky
                                                                          ///< TObjects / CLHEP vectors etc         
    PlaneInfoMap_t<float>                                fWireCounts;     ///< Number of wires in each plane - for
                                                                          ///< range checking after calculation   
    TPCInfoMap_t<unsigned int>                           fNPlanes;        ///< Number of planes in each TPC - for
                                                                          ///< range checking after calculation   
    PlaneInfoMap_t<unsigned int>                         fPlaneBaselines; ///< The number of wires in all the 
                                                                          ///< tpcs and planes up to this one 
                                                                          ///< in the heirachy
    PlaneInfoMap_t<unsigned int>                         fWiresPerPlane;  ///< The number of wires in this plane 
                                                                          ///< in the heirachy

    geo::GeoObjectSorterStandard                         fSorter;         ///< class to sort geo objects
    
    
    /// Retrieved the wire cound for the specified plane ID
    unsigned int WireCount(geo::PlaneID const& id) const
      { return AccessElement(fWireCounts, id); }
    
  };


}
#endif // GEO_CHANNELMAPSTANDARDALG_H

