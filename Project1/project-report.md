# Project 1


## 8) Proof of vectorization
Looking at the dissasembly of the vectorized build of Experiment 1, we can find
the code for Kernel 1 (streamed FMA), and we can see right here as it does
multiply/accumulate in a single instruction, operating on 8 floats at a time
(SIMD with a 256 bit vector width).
```asm
.L102:
# include/experiment-utils.h:88:         output[i] = scalar * arr1[i] + arr2[i];
	vmovaps	(%r15,%rax), %ymm0	# MEM <vector(8) float> [(float *)&f32_l2_arr1 + ivtmp.1232_2094 * 1], vect__460.900
	vfmadd213ps	(%r14,%rax), %ymm1, %ymm0	# MEM <vector(8) float> [(float *)&f32_l2_arr2 + ivtmp.1232_2094 * 1], vect_cst__2230, vect__460.900
# include/experiment-utils.h:88:         output[i] = scalar * arr1[i] + arr2[i];
	vmovaps	%ymm0, (%rbx,%rax)	# vect__460.900, MEM <vector(8) float> [(float *)&f32_l2_out + ivtmp.1232_2094 * 1]
	addq	$32, %rax	#, ivtmp.1232
	cmpq	$2000000, %rax	#, ivtmp.1232
	jne	.L102	#,

```

