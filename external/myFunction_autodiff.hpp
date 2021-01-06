//This version jsut uses autodiff
template <typename T0__, typename T1__>
typename boost::math::tools::promote_args<T0__, T1__>::type

myFunction(const T0__& A, const T1__& B, std::ostream* pstream__) {
  typedef typename boost::math::tools::promote_args<T0__, T1__>::type ReturnType;

  ReturnType returnValue = A * B;

  return returnValue;
}