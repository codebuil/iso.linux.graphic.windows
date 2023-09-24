static unsigned char *memoryStart;
typedef int size_t;
int NULL;
// Estrutura para representar um bitmap com cabeçalho
typedef struct Bitmap {
    int x;          // Comprimento em X
    int y;          // Comprimento em Y
    unsigned char* data; // Ponteiro para os dados do bitmap
} Bitmap;

 void memfill(void *dest, size_t length, unsigned char value) {
    // Cast o ponteiro para unsigned char* para permitir o preenchimento byte a byte
    unsigned char *d = (unsigned char *)dest;

    // Preencha os bytes da memória com o valor especificado
    for (size_t i = 0; i < length; i++) {
        d[i] = value;
    }
}

void memcopy(void *dest, const void *src, size_t length) {
    // Cast os ponteiros para unsigned char* para permitir a cópia byte a byte
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    // Copie os bytes de src para dest
    for (size_t i = 0; i < length; i++) {
        d[i] = s[i];
    }
}
 char *malloc(int length) {
    // Verifica se há espaço suficiente na memória
    unsigned char *memoryEnd = memoryStart + length + sizeof(int) * 2+4;
    if (memoryEnd > (unsigned char *)0x700000) {
        // Não há espaço suficiente
        return (char*)NULL;
    }

    // Preenche o bloco de memória com zeros
    memfill(memoryStart, length + sizeof(int) * 2, 0);

    // Preenche o inteiro do cabeçalho
    *(char *)memoryStart = length;

    // Preenche o inteiro do rodapé
    *(char *)(memoryStart + length + sizeof(int)) = length;
    memoryEnd=(memoryStart + sizeof(int));
    memoryStart= memoryStart+ length + sizeof(int) * 2+8;
    // Retorna o endereço após os inteiros do cabeçalho
    return (char *)memoryEnd;
}

 // Função para contar o tamanho de uma string
size_t lens(const char* str) {
    size_t length = 0;
    while (*str) {
        length++;
        str++;
    }
    return length;
}

// Função para copiar uma string e adicionar seu comprimento ao valor
void strcpys(char* dest, const char* src) {
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0'; // Null-terminator
}
unsigned char getKeys() {
    unsigned char value;
    
    // Usando uma instrução asm inline para ler a porta 0x60
    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(0x60));
    
    return value;
}
void invertScreenRightToLeft() {
    unsigned char *videoMemory = (unsigned char *)0xA0000;
    int screenWidth = 320; // Largura da tela em pixels
    int screenHeight = 200; // Altura da tela em pixels
    int middleLine = screenHeight / 2; // Linha do meio (se ímpar, não se mexe)
    
    unsigned char temp;
    
    for (int y = 0; y < screenHeight; y++) {
        int offset = y * screenWidth;
        
        for (int x = 0; x < screenWidth / 2; x++) {
            // Troca os valores dos pixels da direita para a esquerda
            temp = videoMemory[offset + x];
            videoMemory[offset + x] = videoMemory[offset + (screenWidth - x - 1)];
            videoMemory[offset + (screenWidth - x - 1)] = temp;
        }
    }
}


void pixels(char b,int loc)
        {
        		   int i=loc;
			   char *fbp=(char* )i;
			   	*((char *)(fbp)) =(char)b;
			   		  
        }
void vline(int x,int y,int y1,char b)
{
	int i=0xa0000;
	int xx=y*320+x;
	int yyy=y1-y;
	int xxxx=0;
	int c=0;
	if (y1>=y && x<320 && x>-1 && y>-1 && y<200  && y1>-1 && y1<200)
	{
		
			   
			   for(c=0;c<(yyy*320);c=c+320)
			   	pixels(b,i+c+xx);
	}
	
}
void hline(int x,int y,int x1,char b)
{
	int i=0xa0000;
	int xx=y*320+x;
	int xxx=x1-x;
	int xxxx=0;
	int c=0;
	if (x1>=x && x<320 && x>-1 && y>-1 && y<200 && x1<320 && x1>-1)
	{
		
			   
			   for(c=0;c<xxx;c=c+1)
			   	pixels(b,i+c+xx);
	}
	
}
void box(int x,int y,int x1,int y1,char b)
{
	int n=0;
	int nn=y1-y;
	if (y1>=y && x1>=x && x<320 && x>-1 && y>-1 && y<200 && x1<320 && x1>-1 && y1>-1 && y1<200){
	for (n=0;n<nn;n=n+1)   
			hline(x,n+y,x1,0);
			}
}
 void cls()
 {
 			   int i=0xa0000;
			   char b=1;
			   int c=0;
			   for(c=0;c<65000;c=c+1)
			   	pixels(b,i+c);
 } 
 // Função para criar um bitmap
Bitmap* createBitmap(int x, int y) {
    // Aloca memória para a estrutura Bitmap + dados do bitmap
    Bitmap* bmp = (Bitmap*)malloc(sizeof(Bitmap));
    if (bmp == (void*)NULL) {
        return(void*) NULL; // Falha na alocação de memória
    }

    // Preenche o cabeçalho com as dimensões
    bmp->x = x;
    bmp->y = y;
    bmp->data=(unsigned char*) malloc(x*y+4);
    if (bmp == (void*)NULL) {
        return(void*) NULL; // Falha na alocação de memória
    }
    // Inicializa os dados do bitmap (opcional)
    // Você pode preencher os dados aqui ou em outra função

    return bmp; // Retorna o ponteiro para o bitmap + cabeçalho
}
// Função para desenhar um bitmap na tela
void pbitmap(int x, int y, Bitmap *address) {
    int i, j;
    unsigned char* vga = (unsigned char*)0xA0000; // Endereço da memória de vídeo VGA
    int screenWidth = 320; // Largura da tela no modo 13h

    for (i = 0; i < address->y; i++) {
        for (j = 0; j < address->x; j++) {
            unsigned char pixel = address->data[i * address->x + j];
            vga[(y + i) * screenWidth+ (x + j)] = pixel;
        }
    }
}
void invertScreen() {
    unsigned char *videoMemory = (unsigned char *)0xA0000;
    int screenWidth = 320; // Largura da tela em pixels
    int screenHeight = 200; // Altura da tela em pixels
    int middleLine = screenHeight / 2; // Linha do meio (se ímpar, não se mexe)
    
    unsigned char temp;
    
    for (int y = 0; y < middleLine; y++) {
        int topOffset = y * screenWidth;
        int bottomOffset = (screenHeight - y - 1) * screenWidth;
        
        for (int x = 0; x < screenWidth; x++) {
            // Troca os valores dos pixels de cima para baixo
            temp = videoMemory[topOffset + x];
            videoMemory[topOffset + x] = videoMemory[bottomOffset + x];
            videoMemory[bottomOffset + x] = temp;
        }
    }
}

 int kernel_main()
        {
		Bitmap *bmp;
		char *d;	   
		int n=0;	   
		int x=150;
		int y=150;
		unsigned char* addr;
		cls();
		NULL=0;
		
		memoryStart = (unsigned char *)0x200000;
		for (n=0;n<150;n=n+8)   
			hline(0,n,319,0);
		for (n=0;n<300;n=n+8)   
			vline(n,0,199,0);
		box(150,75,175,100,0);	   	
		
		bmp=createBitmap(x,y);
		addr=(unsigned char*)bmp->data;
		
		memfill(addr,x*y,9);
		pbitmap(3,3,bmp) ;
		invertScreen(); 
		invertScreenRightToLeft(); 
		return 0;	  
        }
 

