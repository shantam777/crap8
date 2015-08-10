	#include<iostream>
	#include<fstream>
	#include<conio.h>
	//#include<graphics.h>
	#include<cstdlib>
	#include<ctime>
	#include<iomanip>
	#include<sstream>

	using namespace std;

	int count1=0;
	unsigned char chip8_fontset[80] =
		{
			0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
			0x20, 0x60, 0x20, 0x20, 0x70, // 1
			0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
			0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
			0x90, 0x90, 0xF0, 0x10, 0x10, // 4
			0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
			0xF0, 0x10, 0x20, 0x40, 0x40, // 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
			0xF0, 0x90, 0xF0, 0x90, 0x90, // A
			0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
			0xF0, 0x80, 0x80, 0x80, 0xF0, // C
			0xE0, 0x90, 0x90, 0x90, 0xE0, // D
			0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
			0xF0, 0x80, 0xF0, 0x80, 0x80  // F
		};


	class Chip8
	{
	      unsigned char memory[4096];
	      unsigned char V[16];
	      unsigned short opcode;
	      unsigned short index;
	      unsigned short  pc;
	      unsigned char gfx[64 * 32];
	      unsigned char delay_timer;
	      unsigned char sound_timer;
	      unsigned char key[16];
	      unsigned short stack[16];
	      unsigned char sp;
	      public:
	      void initialize()
	      {
		       pc=0x200;
		       delay_timer=0;
		       sound_timer=0;
		       opcode=0;
		       index=0;
		       sp=0;
		       for(int i=0;i<80;i++)
		       {
			           memory[i]=chip8_fontset[i];
	           }
	      }
	      void load()
	      {
		       ifstream fs;
		       int i=0,bufferSize;
		       unsigned char buffer[4096];
		       fs.open("pong2.c8", ios::binary);
		       if(fs.is_open())
	                  cout<<"\nSuccess\n";
	           else
	                  cout<<"\nFAIL\n";
		       while(fs)
		       {
			            buffer[i]=fs.get();
			            i++;
	           }
		       fs.close();
		       bufferSize=i;
		       for(i = 0; i < bufferSize; ++i)
			         memory[i + 0x200] = buffer[i];
	      }
	      void emulateCycle()
	      {
		       count1++;
		       opcode= memory[pc]<<8 | memory[pc+1];
		       stringstream ss;
	           ss<< hex << opcode; // int decimal_value
	           string res ( ss.str() );
		       cout<<"\nOpcode being executed : "<<res<<"\n";
		       switch(opcode & 0xF000)
	           {
			   case 0x0000:
				switch(opcode & 0x000F)
				{
					       case 0x0000: //clrscr();
							    pc+=2;
							    break;
					       case 0x000E: pc = stack[--sp];
							    pc+=2;
							    break;
					       default: cout<<"RCA 1802 Program called.\nOpcode was "<<opcode<<".\n";
							    //stack[sp++] = pc;
							    //pc = opcode & 0x0FFF;
							    cout<<"Here the RCA 1802 interpreter would execute the program.\nReturning...\n";
							    //pc=stack[--sp];
							    pc+=2;
							    break;
				}
			   case 0x1000: pc = opcode & 0x0FFF;
					break;
			   case 0x2000: stack[sp++] = pc;
					pc = opcode & 0x0FFF;
					break;
			   case 0x3000: if( V[ (opcode & 0x0F00) >> 8 ] == opcode & 0x00FF )
					      pc+=4;
					else
					      pc+=2;
					break;
			   case 0x4000: if( V[ (opcode & 0x0F00) >> 8 ] != opcode & 0x00FF )
					      pc+=4;
					else
					      pc+=2;
					break;
			   case 0x5000: if( V[ (opcode & 0x0F00) >> 8 ] == V[ (opcode & 0x00F0) >> 4])
					      pc+=4;
					else
					      pc+=2;
					break;
			   case 0x6000: V[ (opcode & 0x0F00) >> 8 ] = (opcode & 0x00FF);
					pc+=2;
					break;
			   case 0x7000: V[ (opcode & 0x0F00) >> 8 ] += (opcode & 0x00FF);
					pc+=2;
					break;
			   case 0x8000:
				switch(opcode & 0x000F)
				{
					       case 0x0000: V[ (opcode & 0x0F00) >> 8] = V[ (opcode & 0x00F0) >> 4];
							    pc+=2;
							    break;
					       case 0x0001: V[ (opcode & 0x0F00) >> 8] = V[ (opcode & 0x0F00) >> 8] | V[ (opcode & 0x00F0) >> 4];
							    pc+=2;
							    break;
					       case 0x0002: V[ (opcode & 0x0F00) >> 8] = V[ (opcode & 0x0F00) >> 8] & V[ (opcode & 0x00F0) >> 4];
							    pc+=2;
							    break;
					       case 0x0003: V[ (opcode & 0x0F00) >> 8] = V[ (opcode & 0x0F00) >> 8] ^ V[ (opcode & 0x00F0) >> 4];
							    pc+=2;
							    break;
					       case 0x0004: if((V[ (opcode & 0x0F00) >> 8] + V[ (opcode & 0x00F0) >> 4])<(V[ (opcode & 0x0F00) >> 8])&&(V[ (opcode & 0x0F00) >> 8] + V[ (opcode & 0x00F0) >> 4])<V[ (opcode & 0x00F0) >> 4])
								   V[15]=1;
							    else
								   V[15]=0;
							    V[ (opcode & 0x0F00) >> 8] = V[ (opcode & 0x0F00) >> 8] + V[ (opcode & 0x00F0) >> 4];
							    pc+=2;
							    break;
					       case 0x0005: if(V[ (opcode & 0x0F00) >> 8] > V[ (opcode & 0x00F0) >> 4])
								   V[15]=1;
							    else
								   V[15]=0;
							    V[ (opcode & 0x0F00) >> 8] = V[ (opcode & 0x0F00) >> 8] - V[ (opcode & 0x00F0) >> 4];
							    pc+=2;
							    break;
					       case 0x0006: V[15] = V[ (opcode & 0x0F00) >> 8] & 1;
							    V[ (opcode & 0x0F00) >> 8] = V[ (opcode & 0x0F00) >> 8] >> 1;
							    pc+=2;
							    break;
					       case 0x0007: if(V[ (opcode & 0x0F00) >> 8] > V[ (opcode & 0x00F0) >> 4])
								   V[15]=0;
							    else
								   V[15]=1;
							    V[ (opcode & 0x0F00) >> 8] = V[ (opcode & 0x00F0) >> 4] - V[ (opcode & 0x0F00) >> 8];
							    pc+=2;
							    break;
					       case 0x000E: V[15] = (V[ (opcode & 0x0F00) >> 8] >> 15) & 1; //CHECK
							    V[ (opcode & 0x0F00) >> 8] = V[ (opcode & 0x0F00) >> 8] << 1;
							    pc+=2;
							    break;
					       default:     cout<<"\nERROR : Unknown opcode 0x"<<opcode<<"!";
							    pc+=2; //RED
	                            getch();
							    break;
				}
			   case 0x9000: if(V[ (opcode & 0x0F00) >> 8] != V[ (opcode & 0x00F0) >> 4])
						 pc+=4;
					else
						 pc+=2;
					break;
			   case 0xA000: index = (opcode & 0x0FFF);
					pc+=2;
					break;
			   case 0xB000: pc = (opcode & 0x0FFF) + V[0];
					break;
			   case 0xC000: srand((int)time(0));
					V[ (opcode & 0x0F00) >> 8 ] = (rand() % 255) & (opcode & 0x00FF);
					pc+=2;
					break;
			   case 0xD000: //STUB
					cout<<"\nOpcode DXYN invoked.\n";
					pc+=2;
					break;
			   case 0xE000:
				switch(opcode & 0x000F)
				{
					       case 0x000E: if(key[V[ (opcode & 0x0F00) >> 8]]!=0)
								      pc+=4;
							    else
								      pc+=2;
							    break;
					       case 0x0001: if(key[V[ (opcode & 0x0F00) >> 8]]==0)
								      pc+=4;
							    else
								      pc+=2;
							    break;
					       default:
						       cout<<"\nERROR : Unknown opcode 0x"<<opcode<<"!";
						       pc+=2; //RED
	                           getch();
	                           break;
				}
			   case 0xF000:
				switch(opcode & 0x00FF)
				{
					       case 0x0007: V[ (opcode & 0x0F00) >> 8] = delay_timer;
							    pc+=2;
							    break;
					       case 0x000A: //STUB
							    pc+=2;
							    break;
					       case 0x0015: delay_timer = V[ (opcode & 0x0F00) >> 8];
							    pc+=2;
							    break;
					       case 0x0018: sound_timer = V[ (opcode & 0x0F00) >> 8];
							    pc+=2;
							    break;
					       case 0x001E: index += V[ (opcode & 0x0F00) >> 8];
							    pc+=2;
							    break;
					       case 0x0029: //STUB
							    pc+=2;
							    break;
					       case 0x0033: memory[index]=V[ (opcode & 0x0F00) >> 8]/100;
							    memory[index+1]=(V[ (opcode & 0x0F00) >> 8]%100)/10;
							    memory[index+2]=V[ (opcode & 0x0F00) >> 8]%10;
							    break;
					       case 0x0055: {
								int i;
								int j;
								for(i=0,j=index;i<=(opcode & 0x0F00) >> 8;i++)
								{
									  memory[j]=V[i];
									  j+=2;
								}
							    }
							    break;
					       case 0x0065: {
								int i;
								int j;
								for(i=0,j=index;i<=(opcode & 0x0F00) >> 8;i++)
								{
									  V[i]=memory[j];
									  j+=2;
								}
							    }
							    break;
					       default:
						       cout<<"\nERROR : Unknown opcode 0x"<<opcode<<"!";
						       pc+=2; //RED
						       getch();
						       break;
				}
			   default:
				 cout<<"\nERROR : Unknown opcode 0x"<<opcode<<"!";
				 pc+=2; //RED
				 getch();
		   }
		   if(delay_timer > 0)
				  delay_timer--;
		   if(sound_timer > 0)
		   {
				  if(sound_timer == 1)
						 cout<<"\nBEEP!";
				  sound_timer--;
		   }
	      }
	      void drawFlag()
	      {
		   //STUB
		   return;
	      }
	      void setKeys()
	      {
		   //STUB
		   return;
	      }
	};
	void setupGraphics()
	{
		//STUB
		return;
	}
	void setupInput()
	{
		//STUB
		return;
	}

	int main(int argc, char* argv[])
	{
	    if(argc!=2)
	    {
		      cout<<"\nUSAGE : chip8.exe <program>\n\n";
		      getch();
		      return 1;
	    }
	    //int gd = DETECT, gm;
	    //initgraph(&gd, &gm, "C:\\TC\\BGI");
	    Chip8 myChip;
	    myChip.initialize();
	    myChip.load();
	    while(1)
	    {
	           myChip.emulateCycle();
	           getch();
	    }
	    //closegraph();
	    cout<<"No. of emulated cycles : "<<count1;
	    getch();
	    return 0;
	}
