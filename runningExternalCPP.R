library(rstan)

#Suppose we need a special function "myFunction" that takes two real-valued
# arguments A and B and returns a real value:
# real myFunction(real A, real B)

#Firstly, we can write the declaration of our function in the "functions" block at
#the top of the Stan file "minimalStanFile.stan" (without implementing it).

#Use 'stanc' to parse our Stan file to generate some C++ code
stanCode = stanc(
  'minimalStanFile.stan',
  allow_undefined=T
)
#Write this to a file
f <- file("generatedCPP.cpp","w")
writeLines(stanCode$cppcode, f)
close(f)

#If you now look in the file 'generatedCPP.cpp'
#you should find a function declaration for 'myFunction'

#  template <typename T0__, typename T1__>
#  typename boost::math::tools::promote_args<T0__, T1__>::type
#  myFunction(const T0__& A,
#             const T1__& B, std::ostream* pstream__);

#This is what your C++ function in 'myFunction.hpp'/'myFunction_autodiff.hpp' should conform to
#pstream___ is a pointer to an output stream that you can use to print things to screen (like error messages)
#that you most likely won't need to use but the function definitions need to have this in their inputs

#The include file need to be the full path since this
#is injected into the C++ before compilation
include_files <- paste0('\n#include "', file.path(getwd(), "external","myFunction_autodiff.hpp"), '"\n')

#Compile the stan model with our external C++ file
stanModel = stan_model(
  'minimalStanFile.stan',
  verbose=F,
  allow_undefined=T,
  includes=include_files
)

fit = sampling(stanModel)
print(fit)


#The above function works fine but relies on Stan's autodiff to get the gradient
#We can do better though by manually calculating the gradient of our function myFunction(A,B)
#This is what allows us to speed things up

#I've done this in a new C++ file
include_files <- paste0('\n#include "', file.path(getwd(), "external","myFunction.hpp"), '"\n')
#There's a few different definitions, not all of which will necessarily be needed
#in all situations. For example, if "A" is always a constant then we don't need to
#worry about the function definitions where "A" is a variable parameter

stanModel = stan_model(
  'minimalStanFile.stan',
  verbose=F,
  allow_undefined=T,
  includes=include_files
)

fit = sampling(stanModel)
print(fit)
