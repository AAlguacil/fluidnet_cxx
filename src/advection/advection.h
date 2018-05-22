#pragma once

#include "calc_line_trace.h"
#include "advect_type.h"
#include "grid/grid.h"

namespace fluid {
// ****************************************************************************
// Advect Scalar
// ****************************************************************************

// Euler advection with line trace (as in Fluid Net)
T SemiLagrangeEulerFluidNet
(
    FlagGrid& flags,
    MACGrid& vel,
    RealGrid& src,
    float dt,
    int order_space,
    int32_t i, int32_t j, int32_t k, int32_t b,
    const bool line_trace,
    const bool sample_outside_fluid
);

// Same kernel as previous one, except that it saves the 
// particle trace position. This is used for the Fluid Net
// MacCormack routine (it does
// a local search around these positions in clamp routine).
T SemiLagrangeEulerFluidNetSavePos
(
    FlagGrid& flags,
    MACGrid& vel,
    RealGrid& src,
    float dt,
    int order_space,
    int32_t i, int32_t j, int32_t k, int32_t b,
    const bool line_trace,
    const bool sample_outside_fluid,
    VecGrid& pos
);

T MacCormackCorrect
(
    FlagGrid& flags,
    const RealGrid& old,
    const RealGrid& fwd,
    const RealGrid& bwd,
    const float strength,
    bool is_levelset,
    int32_t i, int32_t j, int32_t k, int32_t b
);

void getMinMax(T& minv, T& maxv, const T& val) {
  if (toBool(val < minv)) {
    minv = val;
  }
  if (toBool(val > maxv)) {
    maxv = val;
  }
};

// FluidNet clamp routine. It is a search around a single input
// position for min and max values. If no valid values are found, then
// false is returned (indicating that a clamp shouldn't be performed) otherwise
// true is returned (and the clamp min and max bounds are set).
bool getClampBounds
(
    RealGrid src,
    T pos,
    const int32_t b,
    FlagGrid flags,
    const bool sample_outside_fluid,
    T* clamp_min,
    T* clamp_max
) ;

T MacCormackClampFluidNet
(
    FlagGrid& flags,
    MACGrid& vel,
    const RealGrid& dst,
    const RealGrid& src,
    const RealGrid& fwd,
    float dt,
    const VecGrid& fwd_pos,
    const VecGrid& bwd_pos,
    const bool sample_outside_fluid,
    int32_t i, int32_t j, int32_t k, int32_t b
);

// Main routine for scalar advection
void advectScalar
(
    float dt,
    at::Tensor& tensor_flags,
    at::Tensor& tensor_u,
    at::Tensor& tensor_s,
    at::Tensor& tensor_s_dst,
    at::Tensor& tensor_fwd,
    at::Tensor& tensor_bwd,
    at::Tensor& tensor_fwd_pos,
    at::Tensor& tensor_bwd_pos,
    const bool is_3d,
    const std::string method_str,
    const int32_t boundary_width = 1,
    const bool sample_outside_fluid = false,
    const float maccormack_strength = 1.
);

// ****************************************************************************
// Advect Velocity
// ***************************************************************************

T SemiLagrangeEulerFluidNetMAC(
    FlagGrid& flags,
    MACGrid& vel,
    MACGrid& src,
    float dt,
    int order_space,
    const bool line_trace,
    int32_t i, int32_t j, int32_t k, int32_t b);

T SemiLagrangeMAC(
    FlagGrid& flags,
    MACGrid& vel,
    MACGrid& src,
    float dt,
    int order_space,
    int32_t i, int32_t j, int32_t k, int32_t b);

T MacCormackCorrectMAC(
    FlagGrid& flags,
    const MACGrid& old,
    const MACGrid& fwd,
    const MACGrid& bwd,
    const float strength,
    int32_t i, int32_t j, int32_t k, int32_t b) ;

template <int32_t c>
T doClampComponentMAC(
    const T& gridSize,
    T dst,
    const MACGrid& orig,
    T fwd,
    const T& pos, const T& vel,
    int32_t b);

T MacCormackClampMAC(
    FlagGrid& flags,
    MACGrid& vel,
    T dval,
    const MACGrid& orig,
    const MACGrid& fwd,
    float dt,
    int32_t i, int32_t j, int32_t k, int32_t b);

void advectVel
(
    float dt,
    T& tensor_flags,
    T& tensor_u,
    T& tensor_u_dst,
    T& tensor_fwd,
    T& tensor_bwd,
    const bool is_3d,
    const std::string method_str,
    const int32_t boundary_width,
    const float maccormack_strength
);

} // namespace fluid