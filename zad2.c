#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
   int plik;
   int plik_pom;
   char *plik_w_pamieci;
   char plik_wej[100];
   struct stat filestat; // do informacji o pliku, np rozmiar

   if(fork() == 0) // tworzymy proces dziecko
   {
      sleep(1);
      execlp("display", "display", "-update", "1", "-delay", "2", "plik", NULL);
   }

   while(1)
   {
      printf("Podaj nazwe pliku:\n");
      scanf("%s", plik_wej);

      if(( plik = open(plik_wej, O_RDWR, S_IRUSR | S_IWUSR)) < 0)
      {
         printf("Nie ma takiego pliku\n");
      }
      
      plik_pom = open("plik", O_RDWR | O_CREAT);  
      fstat(plik, &filestat);

      truncate("plik", filestat.st_size); // dopasowanie pliku do rozmiaru
      plik_w_pamieci = mmap(NULL, filestat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, plik_pom, 0 );
      read(plik, plik_w_pamieci,filestat.st_size);

      msync(plik_w_pamieci, filestat.st_size, MS_SYNC); // synchronizacja obszaru pamięci z z obrazem pliku na dysku
      munmap(plik_w_pamieci, filestat.st_size);
      close(plik);
   }
}


