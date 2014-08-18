#include <stdio.h>
#include <modbus.h>
#include <errno.h>
#include <unistd.h>

int main()
{
        modbus_t *ctx;
        uint8_t dest[8];
				uint8_t src[4];
				uint16_t tab_reg[10];
				uint16_t spd[5];
	
	int i;


        ctx = modbus_new_rtu("/dev/ttyUSB1", 38400, 'N', 8, 1);
        if (ctx == NULL) {
                fprintf(stderr, "Unable to create the libmodbus context\n");
                return -1;
        }
				modbus_set_debug(ctx,1);
        int ret = modbus_set_slave(ctx,0x36);
        if (ret != 0){
                fprintf(stderr, "Unable to set\n");
                return -1;
        }
        printf("Set Serialmode");
        modbus_rtu_set_serial_mode(ctx,MODBUS_RTU_RS485);
        printf("Connecting...");


        if (modbus_connect(ctx) == -1) {
                fprintf(stderr, "Connexion failed: %s\n", modbus_strerror(errno));
                modbus_free(ctx);
                return -1;
        }

        ret = modbus_read_input_bits(ctx,0,8,dest);
        printf("%d\n",ret);
        if (ret < 0){
                fprintf(stderr, "Unable to read\n");
        }

				ret = modbus_read_input_registers(ctx,0,10,tab_reg);
				printf("%d\n",ret);
				printf("Power In=%d, Iout=%d, Vin=%d, PrcOut=%d, RPMOut=%d, InternalTemp=%d\n",\
					tab_reg[8],tab_reg[7], tab_reg[6], tab_reg[3], tab_reg[4], tab_reg[5]); 
	
				src[0]=1; src[1]=1; src[2]=1; src[3]=1;
				ret = modbus_write_bits(ctx,0,3,src);
				printf("%d\n",ret);


				spd[0]=10000;spd[1]=0;spd[2]=2250;spd[3]=15;spd[4]=15;
				ret = modbus_write_registers(ctx,0,5,spd);
				printf("%d\n",ret);

				ret = modbus_write_register(ctx,10,0);
				printf("%d\n",ret);

			
				//for (i=0; i<5; i++){
				while(1==1){
					sleep(5);
					ret = modbus_read_input_registers(ctx,0,10,tab_reg);
					printf("Power In=%d, Iout=%d, Vin=%d, PrcOut=%d, RPMOut=%d, InternalTemp=%d\n",\
						tab_reg[8],tab_reg[7], tab_reg[6], tab_reg[3], tab_reg[4], tab_reg[5]);
				}

        modbus_close(ctx);
        modbus_free(ctx);
        return 0;
}
