#include "UI.h"

// g++ *.cpp -o test.exe -lsqlite3 -lws2_32 -lcomdlg32

int main(){
    if(LoginWindow() == true){
        isBeginAutoload = true;
        downloadMail();
        
        std::thread autoDownloadThread(autoDownloadMail);
        
        while(true)
            MailBoxWindow();
        
        autoDownloadThread.join();
    }
    return 0;
}







