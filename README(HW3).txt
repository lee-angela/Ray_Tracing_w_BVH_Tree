

CIS460/560 HOMEWORK 3



**NOTE: 
-rendering meshs was not functioning in previous assignment - may affect rendering of mesh's in this assignment as well.

-scenes tested for bounding box functionality : wahoo.xml, teapot.xml, mesh.xml, cornell_box_textured.xml, many_spheres.xml 






Extra Credit

4.2 Image-Wide Stratified Sampling:

- implemented this by generating N samples within desired pixel and choosing the one that is gauranteed to be a minimum distance away from all the other chosen pixels. Kept class variable in .h file keeping track of all already chosen pixels








Recorded Render Times:

1x1 sampling without using BVH tree

   	Uniform Pixel Sampler 
		Cornell Box:
			-took 2,735 milliseconds
		Many Spheres: 
			-took 5,536 milliseconds
		Wahoo: (rendered teapot instead)
			-took 15,545 milliseconds

	Stratified Pixel Sampler 
		Cornell Box:
			-took 2,772 milliseconds
		Many Spheres: 
			-took 5,632 milliseconds
		Wahoo: (rendered teapot instead)
			-took 15,642 milliseconds

	Random Pixel Sampler
		Cornell Box:
			-took 2,771 milliseconds
		Many Spheres: 
			-took 5,795 milliseconds
		Wahoo: (rendered teapot instead)
			-took 15,572 milliseconds

	Image-Wide Stratified Pixel Sampler
		Cornell Box:
			-took 2,764 milliseconds
		Many Spheres: 
			-took 5,619 milliseconds
		Wahoo: (rendered teapot instead)
			-took 15,541 milliseconds

	

1x1 sampling (with BVH)

	Uniform Pixel Sampler 
		Cornell Box:
			-took 9,412 milliseconds
		Many Spheres: 
			-took 18,780 milliseconds
		Wahoo: (rendered teapot instead)
			-took  milliseconds

	Stratified Pixel Sampler 
		Cornell Box:
			-took 9,393 milliseconds
		Many Spheres: 
			-took 18,789 milliseconds
		Wahoo: (rendered teapot instead)
			-took  milliseconds

	Random Pixel Sampler
		Cornell Box:
			-took 9,429 milliseconds
		Many Spheres: 
			-took 18,782 milliseconds
		Wahoo: (rendered teapot instead)
			-took  milliseconds

	Image-Wide Stratified Pixel Sampler
		Cornell Box:
			-took 9,387 milliseconds
		Many Spheres: 
			-took 18,776 milliseconds
		Wahoo: (rendered teapot instead)
			-took 15,480 milliseconds


2x2 sampling (with BVH)

	Uniform Pixel Sampler 
		Cornell Box:
			-took 37,625 milliseconds
		Many Spheres: 
			-took 74,860 milliseconds
		Wahoo: (rendered teapot instead)
			-took 15,550 milliseconds

4x4 sampling (with BVH)
	
	Uniform Pixel Sampler 
		Cornell Box:
			-took 37,376 milliseconds
		Many Spheres: 
			-took 75,426 milliseconds
		Wahoo: (rendered teapot instead)
			-took 15,518 milliseconds






