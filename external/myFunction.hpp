//This implementation assumes A and B are fixed
inline var myFunction(double A, double B, std::ostream* pstream__) {
  //Compute the value
  double result = A * B;
  return result;
}

//This implementation assumes A and B are both non-constant (i.e. they are parameters/functions of parameters)
//which are of type "var" that Stan uses for the autodifferentiation
//Ignore the fact that the C++ syntax says "const"
inline var myFunction(const var& A_var, const var& B_var, std::ostream* pstream__) {

  //Extract the values of A and B
  double A = A_var.val();
  double B = B_var.val();

  //Compute the value using the function above
  double result = myfunction(A, B, pstream__);

  //And the partial derivatives
  double dresult_dA = B;
  double dresult_dB = A;

  //We wrap the result and the partial derivative as a new "var"
  //Note that because this is the gradient wrt multiple parameters, we use precomp_vv_vari with two Vs
  return var(new precomp_vv_vari(
    result,
    A_var.vi_, //Input gradient
    B_var.vi_, //Input gradient
    dresult_dA, //Partial wrt A
    dresult_dB  //Partial wrt B
  ));
}

//This implementation assumes A is non-constant and B is fixed
inline var myFunction(const var& A_var, double B, std::ostream* pstream__) {

  double A = A_var.val();

  //Compute the value
  double result = myfunction(A, B, pstream__);

  //And the partial derivatives
  double dresult_dA = B;

  //We wrap the result and the partial derivative as a new "var"
  //Note that because this is the gradient wrt one parameter, we use precomp_v_vari with one V
  return var(new precomp_v_vari(
    result,
    A_var.vi_, //Input gradient
    dresult_dA //Partial wrt A
  ));
}



//This implementation assumes A is fixed B are is non-constant
inline var myFunction(double A, const var& B_var, std::ostream* pstream__) {

  double B = B_var.val();

  //Compute the value
  double result = myfunction(A, B, pstream__);

  //And the partial derivative
  double dresult_dB = A;

  //We wrap the result and the partial derivative as a new "var"
  return var(new precomp_v_vari(
    result,
    B_var.vi_, //Input gradient
    dresult_dB  //Partial wrt B
  ));
}