////////////////////////////////////////////////////////////////////////
/// \file  ChannelMapAlg.h
/// \brief Interface to algorithm class for a specific detector channel mapping
///
/// \version $Id:  $
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#ifndef GEO_CHANNELMAPALG_H
#define GEO_CHANNELMAPALG_H

#include <vector>
#include <set>
#include <stdint.h>

#include "cetlib/exception.h"
#include "SimpleTypesAndConstants/geo_types.h"
#include "Geometry/CryostatGeo.h"

#include "TVector3.h"

namespace geo{

 class ChannelMapAlg{

 public:

   ChannelMapAlg();
   virtual ~ChannelMapAlg();

   virtual void                     Initialize(std::vector<geo::CryostatGeo*> & cgeo) = 0;
   virtual void                	    Uninitialize() = 0;				   
   virtual std::vector<WireID> 	    ChannelToWire(uint32_t channel)           const = 0;
   virtual uint32_t            	    Nchannels()                               const = 0;

   /**
    * @brief Returns the index of the wire nearset to the specified position
    * @param YPos y coordinate on the wire plane
    * @param ZPos z coordinate on the wire plane
    * @param TPCNo number of TPC
    * @param cstat number of cryostat
    * @return an index interpolation of the two nearest wires
    * @see NearestWireID()
    *
    * Respect to NearestWireID(), this method returns a real number,
    * representing an interpolation between the indices of the two wires closest
    * to the specified point.
    */
   virtual float WireCoordinate(const float& YPos, const float& ZPos,
		                unsigned int    PlaneNo,
		                unsigned int    TPCNo,
		                unsigned int    cstat) const = 0;

   virtual WireID              	    NearestWireID(const TVector3& worldPos,		   
			       	    		     unsigned int    PlaneNo,		   
			       	    		     unsigned int    TPCNo,		   
			       	    		     unsigned int    cstat)   const = 0;
   virtual uint32_t            	    PlaneWireToChannel(unsigned int plane,		   
                               	                       unsigned int wire,		   
                               	                       unsigned int tpc,		   
                               	                       unsigned int cstat)    const = 0;
   virtual View_t	       	    View( uint32_t const channel ) 	      const = 0;
   virtual SigType_t     	    SignalType( uint32_t const channel )      const = 0;
   virtual std::set<View_t>  const& Views()                                   const = 0;
   virtual std::set<PlaneID> const& PlaneIDs()                                const = 0;
   unsigned int                     NearestWire(const TVector3& worldPos,
					   unsigned int    PlaneNo,
					   unsigned int    TPCNo,
					   unsigned int    cstat)        const;

   // These methods retrieve the private fFirstChannel*
   // vectors for testing.
   const std::vector<std::vector<std::vector<uint32_t>>> FirstChannelInNextPlane() const
   { return fFirstChannelInThisPlane; }

   const std::vector<std::vector<std::vector<uint32_t>>> FirstChannelInThisPlane() const
   { return fFirstChannelInNextPlane; }

 protected:

   // These 3D vectors are used in initializing the Channel map.
   // Only a 1D vector is really needed so far, but these are more general.
   std::vector< std::vector<std::vector<uint32_t> > > fFirstChannelInThisPlane;
   std::vector< std::vector<std::vector<uint32_t> > > fFirstChannelInNextPlane;

 };
}
#endif // GEO_CHANNELMAPALG_H

