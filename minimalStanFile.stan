functions {
  real myFunction(real A, real B);
}

parameters{
  real a;
  real b;
  real x;
}

model {
  real mu;

  a ~ normal(0,1);
  b ~ normal(0,1);

  mu = myFunction(a, b);
  x ~ normal(mu, 1);
}