Matrix Matrix::  Cholecky()
{
  Matrix L(height_,width_);
  for (std::size_t i = 0; i < height_; i++) 
  {
    for (std::size_t j = 0; j <= i; j++) 
    {
      float sum = 0;
      for (std::size_t k = 0; k < j; k++)
        sum += L[i][k] * L[j][k];
      if (i == j)
        L[i][j] = sqrt(data_[i,i] - sum);
      else
        L[i][j] = (1.0 / L[j][j] * (data_[i][j] - sum));
    }
  }
  return L;
} 