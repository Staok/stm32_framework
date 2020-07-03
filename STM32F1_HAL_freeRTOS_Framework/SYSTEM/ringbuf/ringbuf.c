#include"ringbuf.h"

RingBuff_Typedef RingBuff_forUSART1 = 
{
	.ad2write = 0,
	.ad2read = 0,
	.writeOverRing = 0,
	.readOverRing = 1,	/*可千万别动！*/
	.is_success = 0
}; 


char WriteDataToRingbuff(RingBuff_Typedef* ringbuf_Struct, char Data)
{
	ringbuf_Struct->data = Data; //把要存的数据先放到结构体里
	if(ringbuf_Struct->ad2write > ringbuf_Struct->ad2read) 
	{
		/*	0	      ad2read(指向第一个有数据前一个的位置)									            198 199
			x x ... x   x	     *       * * * * * * * * * * * * * *     *      x     x     x ... x x  x   x
															         ad2write(指向最后一个数据的位置)
		*/
		ringbuf_Struct->ad2write++;
		if(ringbuf_Struct->ad2write >= ringbuf_MaxNum)
		{
			ringbuf_Struct->ad2write = 0;
			ringbuf_Struct->readOverRing = 0;
			ringbuf_Struct->writeOverRing = 1;
		}
		
		ringbuf_Struct->ringbuf[ringbuf_Struct->ad2write] = ringbuf_Struct->data;
		ringbuf_Struct->is_success = ReturnOK;
		return ringbuf_Struct->is_success;
	}else{ /*此时 ad2write <= ad2read*/
		/*		      												       ad2read(指向第一个有数据的前一个位置)
			* * ... * * *     *     x     x x ... x x x x x x x x x x x x     x     *     * * * * ...  *   *
			0	           ad2write(指向最后一个数据的位置)								                198 199
		*/
		ringbuf_Struct->ad2write++;
		if((ringbuf_Struct->ad2write > ringbuf_Struct->ad2read)&&(!ringbuf_Struct->readOverRing))
		{
			ringbuf_Struct->ad2write--;
			ringbuf_Struct->is_success = ReturnErr;
			return ringbuf_Struct->is_success;
		}else{
			ringbuf_Struct->ringbuf[ringbuf_Struct->ad2write] = ringbuf_Struct->data;
			ringbuf_Struct->is_success = ReturnOK;
			return ringbuf_Struct->is_success;
		}
	}
}
//返回正确后，数据存在ringbuf_Struct->data里面
char ReadDataFromRingbuff(RingBuff_Typedef* ringbuf_Struct)
{
	if(ringbuf_Struct->ad2write >= ringbuf_Struct->ad2read)
	{
		/*	0	        ad2read(指向第一个有数据的前一个位置)									     198 199
			x x ... x      x     *    * * * * * * * * * * * * * *     *     x     x     x ... x x  x   x
															       ad2write(指向最后一个数据的位置)
		*/
		ringbuf_Struct->ad2read++;
		if((ringbuf_Struct->ad2read > ringbuf_Struct->ad2write)&&(!ringbuf_Struct->writeOverRing))
		{
			ringbuf_Struct->ad2read--;
			ringbuf_Struct->is_success = ReturnErr;
			return ringbuf_Struct->is_success;
		}
		ringbuf_Struct->data = ringbuf_Struct->ringbuf[ringbuf_Struct->ad2read];
		ringbuf_Struct->is_success = ReturnOK;
		return ringbuf_Struct->is_success;
	}else{ /*此时ad2write < ad2read*/
		/*		      												        ad2read(指向第一个有数据的前一个位置)
			* * ... * * *     *      x     x x ... x x x x x x x x x x x x     x     *     * * * * ...  *   *
			0	           ad2write(指向最后一个数据的位置)								                 198 199
		*/
		ringbuf_Struct->ad2read++;
		if(ringbuf_Struct->ad2read >= ringbuf_MaxNum)
		{
			ringbuf_Struct->readOverRing = 1;
			ringbuf_Struct->writeOverRing = 0;
			ringbuf_Struct->ad2read = 0;
		}
		
		ringbuf_Struct->data = ringbuf_Struct->ringbuf[ringbuf_Struct->ad2read];
		ringbuf_Struct->is_success = ReturnOK;
		return ringbuf_Struct->is_success;
	}
}


