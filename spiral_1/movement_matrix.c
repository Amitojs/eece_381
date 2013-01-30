#include hello_world.c

void trucks_test(void){

	//Truck 1
		if(truck_1 >= 20){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_1-20, 192, truck_1, 215, 0xFF00, 1);
		}else{
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_1, 215, 0xFF00, 1);
		}
		if(truck_1 >= 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_1-40, 192, truck_1-20, 215, 0xFFFF, 1);
		}else if(truck_1 > 20 && truck_1 < 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_1-20, 215, 0xFFFF, 1);
		}
		if (truck_1 >= 60){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_1-60, 192, truck_1-40, 215, 0xFFFF, 1);
		}else if(truck_1 > 40 && truck_1 < 60){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_1-40, 215, 0xFFFF, 1);
		}

		truck_1=truck_1+1;
		if(truck_1 == 380){truck_1=0;}
		movement_matrix[192/24][(truck_1)/20] = 1;
		if (truck_1 >= 20){movement_matrix[192/24][(truck_1-20)/20] = 1;}
		if (truck_1 >= 40){movement_matrix[192/24][(truck_1-40)/20] = 1;}
	//Truck 2
		if(truck_2 >= 20){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_2-20, 192, truck_2, 215, 0xFF00, 1);
		}else{
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_2, 215, 0xFF00, 1);
		}
		if(truck_2 >= 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_2-40, 192, truck_2-20, 215, 0xFFFF, 1);
		}else if(truck_2 > 20 && truck_2 < 40){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_2-20, 215, 0xFFFF, 1);
		}
		if (truck_2 >= 60){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, truck_2-60, 192, truck_2-40, 215, 0xFFFF, 1);
		}else if(truck_2 > 40 && truck_2 < 60){
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 192, truck_2-40, 215, 0xFFFF, 1);
		}

		truck_2=truck_2+1;
		if(truck_2 == 380){
			truck_2=0;
		}

		movement_matrix[192/24][(truck_2)/20] = 1;
		if (truck_2 >= 20){movement_matrix[192/24][(truck_2-20)/20] = 1;}
		if (truck_2 >= 40){movement_matrix[192/24][(truck_2-40)/20] = 1;}

}