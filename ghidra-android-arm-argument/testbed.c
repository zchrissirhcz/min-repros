
void mcReluCInplace(int width,int height, float slope, float* data)
{
  float *input = data;
  float *output;
  
  if (slope == 0.0) {
    output = input;
    for(int i = 0; i < width * height; i++)
    {
      input = output + 1;
      if (*output < 0.0) {
         *output = 0.0;
      }
    }
    return;
  }
  output = input;
  for (int i = 0; i < width * height; i++)
  {
    input = output + 1;
    if (*output < 0.0) {
      *output *= slope;
    }
  }
  return;
}

int main()
{
    return 0;
}