#include <emmintrin.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;

    //make a local kernal object to speed up memory access
    float local_kernel[KERNX*KERNY];
    for (int i = 0; i < KERNX * KERNY; i++) {
    	local_kernel[i] = kernel[i];
    }

    //printf("Start!\n");

    // main convolution loop
 	#pragma omp parallel
 	{
 	#pragma omp for firstprivate(kern_cent_Y, kern_cent_X)
	for(int y = kern_cent_Y; y < data_size_Y-kern_cent_Y; y++){ // the y coordinate of theoutput location we're focusing on
		int x;
		for(x = kern_cent_X; x < kern_cent_X + (data_size_X-2*kern_cent_X)/4*4; x=x+4){ // the x coordinate of the output location we're focusing on
			__m128 out_temp = _mm_setzero_ps();
		for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
			for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
				
						__m128 in_vec = _mm_loadu_ps((const float*)(in+(x+i) + (y+j)*data_size_X));
				        __m128 kernal_vec = _mm_load1_ps((const float*)local_kernel+((kern_cent_X-i)+(kern_cent_Y-j)*KERNX));

						out_temp = 
								_mm_add_ps(out_temp, _mm_mul_ps(in_vec, kernal_vec));
					
				}
			}

			_mm_storeu_ps(out+(x+y*data_size_X),out_temp);
		}

		//SSE tail loop
		//printf("Reached tail\n");
		for(x = kern_cent_X + (data_size_X-2*kern_cent_X)/4*4; x < data_size_X-kern_cent_X; x++){ // the x coordinate of the output location we're focusing on
			float out_temp = 0;
		for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
			for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
				
					out_temp += 
								local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
				}
			}

			out[x+y*data_size_X] = out_temp;
			//printf("Passed %d\n", x);
		}
	}
}
	//printf("After main loop!\n");
	/*Have a loop for each edge case (top/left/right/bottom) along with corners; modify i and j  */

	//top-left corner (top row and left-most column of kernal excluded)

	for(int i = -kern_cent_X+kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
		int x = 0; int y = 0; //x and y stays constant as you are getting the top-left corner
		for(int j = -kern_cent_Y+kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
			out[x+y*data_size_X] += 
					local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
			

		}
	}
 
	//printf("After top-left corner loop!\n");
	//top-right corner (top row and right-most column of kernal excluded)

	for(int i = -kern_cent_X; i <= kern_cent_X-kern_cent_X; i++){ // kernel unflipped x coordinate
		int x = data_size_X-kern_cent_X; int y = 0; //x and y stays constant as you are getting the top-right corner
		for(int j = -kern_cent_Y+kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
			out[x+y*data_size_X] += 
					local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
		}
	}

	//bottom-left corner (first i and j excluded)

	for(int i = -kern_cent_X+kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
		int x = 0; int y = data_size_Y-kern_cent_Y; //x and y stays constant as you are getting the bottom-left corner
		for(int j = -kern_cent_Y; j <= kern_cent_Y-kern_cent_Y; j++){ // kernel unflipped y coordinate
			out[x+y*data_size_X] += 
					local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
		}
	}

	//bottom-right corner (first i and j excluded)

	for(int i = -kern_cent_X; i <= kern_cent_X-kern_cent_X; i++){ // kernel unflipped x coordinate
		int x = data_size_X-kern_cent_X; int y = data_size_Y-kern_cent_Y; //x and y stays constant as you are getting the bottom-right corner
		for(int j = -kern_cent_Y; j <= kern_cent_Y-kern_cent_Y; j++){ // kernel unflipped y coordinate
			out[x+y*data_size_X] += 
					local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
		}
	}

	//printf("Passed corner loops!\n");
	//top row (top row of kernal is excluded)
#pragma omp parallel
 	{
 	#pragma omp for firstprivate(kern_cent_Y, kern_cent_X)
 		for (int y = 0; y < kern_cent_Y; y++) {
 			int x;
	for (x = kern_cent_X; x < kern_cent_X+(data_size_X-2*kern_cent_X)/4*4; x+=4) {
		//for (int y = 0; y < kern_cent_Y; y++) { //y stays constant as you only need to access the top row of y's
		__m128 out_temp = _mm_setzero_ps();
		for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
			for(int j = -kern_cent_Y+kern_cent_X; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate skips first row
				__m128 in_vec = _mm_loadu_ps((const float*)(in+(x+i) + (y+j)*data_size_X));
                __m128 kernal_vec = _mm_load1_ps((const float*)local_kernel+((kern_cent_X-i)+(kern_cent_Y-j)*KERNX));
                out_temp = 
								_mm_add_ps(out_temp, _mm_mul_ps(in_vec, kernal_vec));
				//out[x+y*data_size_X] += 
						//local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
				}
			} _mm_storeu_ps(out+(x+y*data_size_X),out_temp);


			

			}for (x =kern_cent_X+(data_size_X-2*kern_cent_X)/4*4; x<kern_cent_X+(data_size_X-2*kern_cent_X); x++){
				float out_temp = 0;
				for (int i = -kern_cent_X; i <= kern_cent_X; i++){
					for(int j = -kern_cent_Y+kern_cent_X; j <= kern_cent_Y; j++){ 
						out_temp += 
								local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];

					}

				}out[x+y*data_size_X] = out_temp;
		}
	}
}

	//printf("Passed top row loops!\n");

	//bottom row (last row of kernal is excluded)
#pragma omp parallel
 	{
 	#pragma omp for firstprivate(kern_cent_Y, kern_cent_X)
 		for (int x = kern_cent_X; x < data_size_X-kern_cent_X; x++) {
		for (int y = data_size_Y-kern_cent_Y; y < data_size_Y; y++) {  //y stays constant as you only need to access the bottom row of y's
		for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
			for(int j = -kern_cent_Y; j <= kern_cent_Y-kern_cent_Y; j++){ // kernel unflipped y coordinate stops before last row
				out[x+y*data_size_X] += 
							local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
				}
			}
		}
	}
}

	//printf("Passed bottom row loops!\n");

	//left column (first column of kernal is excluded)
#pragma omp parallel
 	{
 	#pragma omp for firstprivate(kern_cent_Y, kern_cent_X)
	for (int y = kern_cent_Y; y < data_size_Y-kern_cent_Y; y++) {
		for (int x = 0; x < kern_cent_X; x++) {   //x stays constant as you only need to access the left-most column of x's
		for(int i = -kern_cent_X+kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate stops before last column
			for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
				out[x+y*data_size_X] += 
						local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
				}
			}
		}
	}
}

	//printf("Passed left column loops!\n");

	//right column (last column of kernal is excluded)
	#pragma omp parallel
 	{
 	#pragma omp for firstprivate(kern_cent_Y, kern_cent_X)
	for (int y = kern_cent_Y; y < data_size_Y-kern_cent_Y; y++) {
		for (int x = data_size_X-kern_cent_X; x < data_size_X; x++) {  //x stays constant as you only need to access the right-most column of x's
		for(int i = -kern_cent_X; i <= kern_cent_X-kern_cent_X; i++){ // kernel unflipped x coordinate stops before last column
			for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
				out[x+y*data_size_X] += 
						local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
				}
			}
		}
	}
}
	

	//printf("Passed all loops!\n");

	return 1;
}
