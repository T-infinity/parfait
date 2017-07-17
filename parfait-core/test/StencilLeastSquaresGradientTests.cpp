#include <catch.hpp>
#include <LeastSquaresGradient.h>

TEST_CASE("Gram-Schmidt weight calculation, unit directions, check diagon"){
    std::vector<Parfait::Point<double>> points = {{1,0,0},{0,1,0},{0,0,1}};
    Parfait::Point<double> center = {0,0,0};
    auto gs = calcGramSchmidtWeights(points, center);
    REQUIRE(gs.r11 == Approx(1));
    REQUIRE(gs.r12 == Approx(0));
    REQUIRE(gs.r13 == Approx(0));
    REQUIRE(gs.r22 == Approx(1));
    REQUIRE(gs.r23 == Approx(0));
    REQUIRE(gs.r33 == Approx(1));
}

TEST_CASE("Gram-Schmidt weight calculation, x and z off diag"){
    std::vector<Parfait::Point<double>> points = {{0,0,0},{1,1,0},{0,0,1}};
    Parfait::Point<double> center = {1,0,0};
    auto gs = calcGramSchmidtWeights(points, center);
    REQUIRE(gs.r11 == Approx( 1.41421));
    REQUIRE(gs.r12 == Approx(0));
    REQUIRE(gs.r13 == Approx(-0.70711));
    REQUIRE(gs.r22 == Approx(1));
    REQUIRE(gs.r23 == Approx(0));
    REQUIRE(gs.r33 == Approx(0.70711));
}

TEST_CASE("Least Squares gradient calculation"){
    std::vector<Parfait::Point<double>> points = {{1,0,0},{0,1,0},{0,0,1}};
    Parfait::Point<double> center = {0,0,0};
    std::vector<double> u = {5, 1, 3}; // 5x + y + 3z

    auto gs= calcGramSchmidtWeights(points, center);
    REQUIRE(gs.r11 == Approx(1));
    REQUIRE(gs.r12 == Approx(0));
    REQUIRE(gs.r13 == Approx(0));
    REQUIRE(gs.r22 == Approx(1));
    REQUIRE(gs.r23 == Approx(0));
    REQUIRE(gs.r33 == Approx(1));

    double u0 = 0;
    auto grad = calcLeastSquaresGradient(points, center, gs, u, u0);
    REQUIRE(grad[0] == Approx(5));
    REQUIRE(grad[1] == Approx(1));
    REQUIRE(grad[2] == Approx(3));
}

//  pure function lstgs_func(dx, dy, dz, r11, r12, r13, r22, r23, r33)
//
//    use kinddefs, only     : dp
//
//    real(dp), intent(in)   :: dx, dy, dz, r11, r12, r13, r22, r23, r33
//    real(dp), dimension(3) :: lstgs_func
//
//    real(dp)               :: r11inv, r22inv, w11, w22, w33
//    real(dp)               :: r12r11, r13r11, r23r22, rmult
//    real(dp)               :: xt1, xt2, xt3, t1, t2, coef1, coef2
//
//    real(dp), parameter    :: my_1 = 1.0_dp
//
//    continue
//
//    r11inv = my_1/r11
//    r22inv = my_1/r22
//
//    w11  = r11inv*r11inv
//    w22  = r22inv*r22inv
//    w33  = my_1/(r33*r33)
//    r12r11 = r12*r11inv
//    r13r11 = r13*r11inv
//    r23r22 = r23*r22inv
//    rmult  = (r12r11*r23r22 - r13r11)*w33
//
//    xt1 = dx*r12r11
//    xt2 = dx*r13r11
//    xt3 = dx*w11
//
//    t1 = w22*r12r11
//    t2 = r23r22*w33
//
//    coef1 = dy - xt1
//    coef2 = dz - xt2 - r23r22*coef1
//
//    lstgs_func(1) = xt3 - t1*coef1 + rmult*coef2
//    lstgs_func(2) = w22*coef1 - t2*coef2
//    lstgs_func(3) = w33*coef2
//
//  end function lstgs_func

//  subroutine lstgs(turb_grad_flag,                                             &
//                   nnodes0,nnodes01,nedgeloc,eptr,symmetry,qnode,              &
//                   gradx,grady,gradz,x,y,z,                                    &
//                   r11,r12,r13,r22,r23,r33,                                    &
//                   n_tot,n_grd,turb,n_turb,eqn_set,ndim,weighted )
//
//    use solution_types,       only : generic_gas
//
//    integer, intent(in) :: turb_grad_flag, nnodes0, nnodes01, n_tot, n_grd
//    integer, intent(in) :: n_turb, nedgeloc
//    integer, intent(in) :: eqn_set, ndim
//
//    integer,  dimension(2,nedgeloc),      intent(in)  :: eptr
//    integer,  dimension(nnodes01),        intent(in)  :: symmetry
//    real(dp), dimension(n_tot,nnodes01),  intent(in)  :: qnode
//    real(dp), dimension(n_turb,nnodes01), intent(in)  :: turb
//    real(dp), dimension(nnodes01),        intent(in)  :: x,y,z
//    real(dp), dimension(nnodes0),         intent(in)  :: r11,r12,r13
//    real(dp), dimension(nnodes0),         intent(in)  :: r22,r23,r33
//    real(dp), dimension(n_grd,nnodes01),  intent(out) :: gradx,grady,gradz
//    logical,  optional,                   intent(in)  :: weighted
//
//    integer :: j, n, node1, node2, nedge_eval
//    integer :: n_vec, nqq
//
//    real(dp)                   :: dx, dy, dz, weight
//    real(dp), dimension(3)     :: terms
//    real(dp), dimension(n_grd) :: dqq, contx, conty, contz
//
//    real(dp), parameter :: my_0 = 0.0_dp
//    real(dp), parameter :: my_1 = 1.0_dp
//
//    continue
//
//    if ( eqn_set == generic_gas ) then
//      n_vec = ndim
//      nqq   = ndim
//    else
//      n_vec = n_tot
//      nqq   = n_grd
//    end if
//
//!   Set distance weighting factor
//
//    weight_factor = my_0
//    if (present( weighted )) then
//      if(weighted) weight_factor = my_1
//    endif
//
//!   Zero out the gradients
//
//    gradx(:,:) = my_0
//    grady(:,:) = my_0
//    gradz(:,:) = my_0
//
//!   Initialize dqq
//
//    dqq(:) = my_0
//
//    nedge_eval = nedgeloc + nedge_ns
//
//!   If second order, loop over all the faces accumulate sums
//
//      scan_edges : do n = 1, nedge_eval
//
//        if ( n <= nedgeloc ) then
//          node1 = eptr(1,n)
//          node2 = eptr(2,n)
//        else
//          node1 = eptr_ns(1,n-nedgeloc)
//          node2 = eptr_ns(2,n-nedgeloc)
//        endif
//
//        dx = x(node2) - x(node1)
//        dy = y(node2) - y(node1)
//        dz = z(node2) - z(node1)
//
//        weight = reconstruct_weight(dx,dy,dz, weight_factor, wls_inv_dist_scale)
//
//        dx = weight*dx
//        dy = weight*dy
//        dz = weight*dz
//
//        dqq(1:n_vec) = weight*(qnode(1:n_vec,node2) - qnode(1:n_vec,node1))
//
//!       turb gradients if needed
//
//        if ( turb_grad_flag > 0 ) then
//
//              do j=1,n_turb
//                dqq(n_tot+j) = weight*( turb(j,node2) - turb(j,node1) )
//              end do
//
//        end if
//
//        if (node1 <= nnodes0) then
//
//          terms(:) = lstgs_func(dx,         dy,         dz,                    &
//                                r11(node1), r12(node1), r13(node1),            &
//                                r22(node1), r23(node1), r33(node1))
//          contx(:) = terms(1)
//          conty(:) = terms(2)
//          contz(:) = terms(3)
//
//          call lstgs_sym(symmetry(node1),                                      &
//                         dx,         dy,         dz,                           &
//                         r11(node1), r12(node1), r13(node1),                   &
//                         r22(node1), r23(node1), r33(node1),                   &
//                         nqq, contx(1:nqq), conty(1:nqq), contz(1:nqq) )
//
//          gradx(1:nqq,node1) = gradx(1:nqq,node1) + contx(1:nqq)*dqq(1:nqq)
//          grady(1:nqq,node1) = grady(1:nqq,node1) + conty(1:nqq)*dqq(1:nqq)
//          gradz(1:nqq,node1) = gradz(1:nqq,node1) + contz(1:nqq)*dqq(1:nqq)
//
//        end if
//
//!       Now do the other node
//
//      end do scan_edges
//
//  end subroutine lstgs


//  subroutine sumgs_nobound(nnodes0,nnodes01,nedgeloc,eptr,x,y,z,               &
//                           r11,r12,r13,r22,r23,r33,bctag)
//
//    integer,                         intent(in)  :: nnodes0, nnodes01
//    integer,                         intent(in)  :: nedgeloc
//    integer,  dimension(2,nedgeloc), intent(in)  :: eptr
//    real(dp), dimension(nnodes01),   intent(in)  :: x, y, z
//    real(dp), dimension(nnodes0),    intent(out) :: r11, r12, r13
//    real(dp), dimension(nnodes0),    intent(out) :: r22, r23, r33
//    integer,  dimension(nnodes01),   intent(in)  :: bctag
//
//    integer :: i, n, node1, node2
//
//    real(dp) :: dx, dy, dz, weight, w2
//
//    real(dp), parameter :: my_0 = 0.0_dp
//
//    real(dp), parameter :: zero_tol = 1.0e-32_dp
//
//  continue
//
//! Set distance weighting factor (assume unweighted)
//
//   !weight_factor = my_0
//
//! Initialize all the rij to 0.0
//
//    do i = 1,nnodes0
//      r11(i) = my_0
//      r12(i) = my_0
//      r13(i) = my_0
//      r22(i) = my_0
//      r23(i) = my_0
//      r33(i) = my_0
//    end do
//
//! Now loop over the edges and accumulate the r's
//
//    scan_edges_1 : do n = 1, nedgeloc
//
//      node1 = eptr(1,n)
//      node2 = eptr(2,n)
//
//      if ( bctag(node1) == 1 ) cycle scan_edges_1
//      if ( bctag(node2) == 1 ) cycle scan_edges_1
//
//      dx = x(node2) - x(node1)
//      dy = y(node2) - y(node1)
//      dz = z(node2) - z(node1)
//
//      w2 = 1._dp
//
//      if(node1 <= nnodes0) then
//        r11(node1) = r11(node1) + dx*dx*w2
//        r12(node1) = r12(node1) + dx*dy*w2
//        r13(node1) = r13(node1) + dx*dz*w2
//      endif
//
//      dx = -dx
//      dy = -dy
//      dz = -dz
//
//      if(node2 <= nnodes0) then
//        r11(node2) = r11(node2) + dx*dx*w2
//        r12(node2) = r12(node2) + dx*dy*w2
//        r13(node2) = r13(node2) + dx*dz*w2
//      endif
//
//    end do scan_edges_1
//
//! Now calculate ||x|| = r11 by taking the square root
//! Also divide r12 and r13 by ||x||
//
//    do i = 1,nnodes0
//      if ( bctag(i) == 0 .and. abs(r11(i))>zero_tol ) then
//        r11(i) = sqrt(r11(i))
//        r12(i) = r12(i)/r11(i)
//        r13(i) = r13(i)/r11(i)
//      end if
//    end do
//
//! Now calculate r22 and r23
//
//    scan_edges_2 : do n = 1, nedgeloc
//
//      node1 = eptr(1,n)
//      node2 = eptr(2,n)
//
//      if ( bctag(node1) == 1 ) cycle scan_edges_2
//      if ( bctag(node2) == 1 ) cycle scan_edges_2
//
//      dx = x(node2) - x(node1)
//      dy = y(node2) - y(node1)
//      dz = z(node2) - z(node1)
//
//      weight = 1._dp
//
//      dx = weight*dx
//      dy = weight*dy
//      dz = weight*dz
//
//      if( node1 <= nnodes0 ) then
//        if ( abs(r11(node1)) > zero_tol ) then
//          r22(node1) = r22(node1) +    (dy - dx*r12(node1)/r11(node1))**2
//          r23(node1) = r23(node1) + dz*(dy - dx*r12(node1)/r11(node1))
//        end if
//      end if
//
//      dx = -dx
//      dy = -dy
//      dz = -dz
//
//      if( node2 <= nnodes0 ) then
//        if ( abs(r11(node2)) > zero_tol ) then
//          r22(node2) = r22(node2) +    (dy - dx*r12(node2)/r11(node2))**2
//          r23(node2) = r23(node2) + dz*(dy - dx*r12(node2)/r11(node2))
//        end if
//      end if
//
//    end do scan_edges_2
//
//! Now finish getting r22 and r23
//
//    do i = 1,nnodes0
//      if ( bctag(i) == 0 .and. abs(r22(i))>zero_tol ) then
//        r22(i) = sqrt(r22(i))
//        r23(i) = r23(i)/r22(i)
//      end if
//    end do
//
//! Now all we have to do is get r33
//
//    scan_edges_3 : do n = 1, nedgeloc
//
//      node1 = eptr(1,n)
//      node2 = eptr(2,n)
//
//      if ( bctag(node1) == 1 ) cycle scan_edges_3
//      if ( bctag(node2) == 1 ) cycle scan_edges_3
//
//      dx = x(node2) - x(node1)
//      dy = y(node2) - y(node1)
//      dz = z(node2) - z(node1)
//
//      weight = 1._dp
//
//      dx = weight*dx
//      dy = weight*dy
//      dz = weight*dz
//
//      if( node1 <= nnodes0 ) then
//        if ( abs(r11(node1))>zero_tol .and. abs(r22(node1))>zero_tol ) then
//          r33(node1) = r33(node1) +          (dz - dx*r13(node1)/r11(node1)    &
//                     - r23(node1)/r22(node1)*(dy - dx*r12/r11))**2
//        end if
//      end if
//
//      dx = -dx
//      dy = -dy
//      dz = -dz
//
//      if( node2 <= nnodes0 ) then
//        if ( abs(r11(node2))>zero_tol .and. abs(r22(node2))>zero_tol ) then
//          r33(node2) = r33(node2) +          (dz - dx*r13(node2)/r11(node2)    &
//                     - r23(node2)/r22(node2)*(dy - dx*r12(node2)/r11(node2)))**2
//        end if
//      end if
//
//    end do scan_edges_3
//
//! Now just get the magnitude of r33
//
//    do i = 1,nnodes0
//      if ( bctag(i) == 0 ) then
//        r33(i) = sqrt(r33(i))
//      endif
//    end do
//
//  end subroutine sumgs_nobound

