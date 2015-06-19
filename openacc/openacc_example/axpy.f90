subroutine axpy(n,alpha,x,y)
  integer,intent(in)::n
  real(kind(0d0)),intent(in):: alpha
  real(kind(0d0)), intent(in) :: x(n)
  real(kind(0d0)), intent(inout) :: y(n)

  integer i

  do i=1,n
     y(i)=y(i)+alpha*x(i)
  enddo

end subroutine axpy
!------------------------------------------

program main
  use util

  implicit none

  integer pow,n,err,i
  real(kind(0d0)), dimension(:),allocatable :: x,y
  real(kind(0d0)) :: axpy_start,time_axpy

  pow=read_arg(1,16)
  n=2**pow
  print*,'memcopy and daxpy test of size ',n

  allocate(x(n),y(n),stat=err)
  if(err/=0)then
     stop'Error in allocation!'
  endif
  
  x(:)=1.5d0
  y(:)=3.0d0

  axpy_start=get_time()
  call axpy(n,2d0,x,y)
  time_axpy=get_time() - axpy_start

  print*,'-------'
  print*,'timings'
  print*,'-------'
  print*,'axpy    : ',time_axpy   ,'s'
  print*,' '

! check for errors
  err=0
  do i=1,n
     if(abs(6d0-y(i)) > 1d15) err=err+1
  enddo

  if(err > 0)then
     print*,'============ FAILED with ',err,' errors'
  else
     print*,'============ PASSED'
  endif

  deallocate(x,y)

end program main
