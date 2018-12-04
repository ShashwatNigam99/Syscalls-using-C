#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

const int SZREAD = 10000;
// function that prints the permissions for any file/directory
void printer(struct stat fileStat)
{
   write(1,"User has read permission:",25);
   write(1,(fileStat.st_mode & S_IRUSR) ? "Yes\n" : "No\n",4);
   write(1,"User has write permission:",26);
   write(1,(fileStat.st_mode & S_IWUSR) ? "Yes\n" : "No\n",4);
   write(1,"User has execute permission:",28);
   write(1,(fileStat.st_mode & S_IXUSR) ? "Yes\n" : "No\n",4);

   write(1,"Group has read permission:",26);
   write(1,(fileStat.st_mode & S_IRGRP) ? "Yes\n" : "No\n",4);
   write(1,"Group has write permission:",27);
   write(1,(fileStat.st_mode & S_IWGRP) ? "Yes\n" : "No\n",4);
   write(1,"Group has execute permission:",29);
   write(1,(fileStat.st_mode & S_IXGRP) ? "Yes\n" : "No\n",4);

   write(1,"Others have read permission:",28);
   write(1,(fileStat.st_mode & S_IROTH) ? "Yes\n" : "No\n",4);
   write(1,"Others have write permission:",29);
   write(1,(fileStat.st_mode & S_IWOTH) ? "Yes\n" : "No\n",4);
   write(1,"Others have execute permission:",31);
   write(1,(fileStat.st_mode & S_IXOTH) ? "Yes\n" : "No\n",4);

   write(1,"\n\n",2);
}

int main(int argc, const char *argv[]) {
	// frip - i/p file descriptor, frop -o/p file descriptor
        int frip,frop;

        const char usagemsg[] = "Usage: ./a.out [filename]\n";
        const char errormsg[] = "Open Error !\n";
        const char poserr[] = "Positioning Error!\n";
        const char readerr[] = "Read Error!\n";
        const char yes[] = "Yes\n";
	const char no[] = "No\n";
        const char symlinkmsg[] = "Checking whether symlink has been created :";
        const char filemsg[] = "Checking whether file has been created :";
	const char dirmsg[] = "Checking whether directory has been created :";
        const char checkmsg[] = "Checking whether file contents have been reversed and case-inverted :";

        // Check whether proper number of arguments has been provided
        if(argc != 2){
          write(2, usagemsg, sizeof(usagemsg)-1);
          return 0;
        }
        struct stat fileStat;
        //checking for directory
        write(1,dirmsg,sizeof(dirmsg)-1);
        if(stat("Assignment",&fileStat) < 0)
              write(1,no,sizeof(no)-1);
        else{
        write(1,yes,sizeof(yes)-1);
        write(1,"Permissions for Assignment folder\n",34);
        printer(fileStat);
        }
        // checking for output file
        write(1,filemsg,sizeof(filemsg)-1);
        if(stat("Assignment/output.txt",&fileStat) < 0)
              write(1,no,sizeof(no)-1);
        else{
        write(1,yes,sizeof(yes)-1);
        write(1,"Permissions for output file\n",34);
        printer(fileStat);
      }
        // creation of symlink
        int link = symlink("Assignment/output.txt","Assignment/link");
        if(link==0){
              write(1,symlinkmsg,sizeof(symlinkmsg)-1);
	            write(1,yes,sizeof(yes)-1);
              lstat("Assignment/link",&fileStat);
              printer(fileStat);
            }
          else{
              write(1,symlinkmsg,sizeof(symlinkmsg)-1);
	            write(1,no,sizeof(no)-1);
            }
         // opening the input file
            frip = open(argv[1], O_RDONLY);//open file to read
            if (frip < 0) {
                write(2, errormsg, sizeof(errormsg)-1);
                exit(1);
            }
         //opening the output file
            frop = open("Assignment/output.txt", O_RDONLY);//open file to read
                if (frop<0){
                write(2, errormsg, sizeof(errormsg)-1);
                exit(1);
            }

            int offset;
            offset = lseek(frip, 0, SEEK_END);//go to the end of the input file
            if(offset<0){
              write(2, poserr, sizeof(poserr)-1);
            }

            char ipbuffer[SZREAD+1];
            char opbuffer[SZREAD+1];
            size_t szrip,szrop;
            int tbread=SZREAD;
            int flag=0;
            while (offset > 0 && flag == 0) {  //while it is not the beginning of the file

                  if(offset<SZREAD)
                      tbread=offset;

                 //go back specified number of bytes in input file
                  lseek(frip, -tbread, SEEK_CUR);
                 //read a chunk of the file
                  szrip = read(frip, ipbuffer, tbread);
                  szrop = read(frop, opbuffer, tbread);

                  if(szrip < 0 || szrop < 0){
                		// read returns -1 if there was an error reading the file
                    write(2, readerr, sizeof(readerr)-1);
                		exit(1);
                  }

                  for(int i=0;i<tbread;++i)
                      {
                         int ipch = ipbuffer[tbread-1-i];
                         if(ipch>=97 && ipch<=122)
                            ipch-=32;
                         else if(ipch>=65 && ipch<=90)
                            ipch+=32;
                        int opch = opbuffer[i];
			// check if the correct case inverted character exists at the reversed position
                         if(opch!=ipch){
                          flag=1;
                          break;
                        }
                      }
		  // go ahead specified bytes in output file
                  lseek(frop, +tbread, SEEK_CUR);
                  offset-=tbread;//track the current offset
            }

            close(frip);
            close(frop);

            write(1,checkmsg,sizeof(checkmsg)-1);
            if(flag==0)
               write(1,yes,sizeof(yes)-1);
            else
               write(1,no,sizeof(no)-1);

        return 0;
      }
