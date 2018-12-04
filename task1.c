#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
// buffer size 
const int SZREAD = 10000;

int main(int argc, const char *argv[]) {
        int fdr, fdw;    //file descriptors, r-read , w- write
        char c;        //holds read char
        int offset;     //current offset
        const char errormsg[] = "Open Error !\n";
        const char createrr[] = "Create Error!\n";
        const char readerr[] = "Read Error!\n";
        const char usagemsg[] = "Usage: ./a.out [filename]\n";
        const char poserr[] = "Positioning Error!\n";

        // Check whether proper number of arguments has been provided
        if(argc != 2){
      		write(2, usagemsg, sizeof(usagemsg)-1);
      		return 0;
      	}
        // Open the file whose contents have to be read ( get file descriptor)
        fdr = open(argv[1], O_RDONLY);//open file to read
        // Check if the file hasnt been opened properly
        if (fdr < 0) {
            write(2, errormsg, sizeof(errormsg)-1);
            exit(1);
        }
        // Make the directory where the output file is going to be stored
        // Check if the driectory was created successfully
        int dir = mkdir("Assignment",0700);//permissions
        if (dir < 0){
             write(2, createrr, sizeof(errormsg)-1);
             exit(1);
         }

        // Make a output file in the newly created directory
        fdw = open("Assignment/output.txt", O_WRONLY|O_CREAT, 0600);
        //open file to write with rw- --- ---
        if(fdw < 0) {
            write(2, errormsg, sizeof(errormsg)-1);
        }

        // reposition the offset of the already open file to be read from to the end of the file
        offset = lseek(fdr, 0, SEEK_END);//go to the end of the file
        if(offset<0){
          write(2, poserr, sizeof(poserr)-1);
        }

        char buffer[SZREAD+1];
        size_t size_read;
        int tbread=SZREAD;

        while (offset > 0) {  //while it is not the beginning of the file

              if(offset<SZREAD)
                  tbread=offset;//account for reading the last remaining chunk of the file

             //go back specified number of spots
              lseek(fdr, -tbread, SEEK_CUR);
             //read a chunk of the file
              size_read = read(fdr, buffer, tbread);
              if(size_read < 0){
            		// read returns -1 if there was an error reading the file
                write(2, readerr, sizeof(readerr)-1);
            		exit(1);
              }

              // Invert case of alphabets
              for(int i=0;i<tbread;++i)
                  {
                     int ch = buffer[i];
                     if(ch>=97 && ch<=122)
                        buffer[i]-=32;
                     else if(ch>=65 && ch<=90)
                        buffer[i]+=32;
                  }
              // reverse the current buffer
              for(int i=0;i<tbread/2;++i)
                  {
                     char temp = buffer[i];
                     buffer[i]= buffer[tbread-1-i];
                     buffer[tbread-i-1]=temp;
                  }
                write(fdw, buffer, size_read);  //write the chunk after processing
                offset-=tbread;     //track the current offset
        }
	// close both the files after work is done
        close(fdr);
        close(fdw);

        return 0;

}
