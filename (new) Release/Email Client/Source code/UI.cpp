#include "UI.h"
#include "SMTPClient.h"

bool LoginWindow(){
    // Load data
    std::cout << "Welcom to Email Cliemt" << std::endl;
    std::cout << "Loading data..." << std::endl;

    CreateDatabase();
    CreateWorkTable();
    CreateImportantTable();
    CreateSpamTable();
    CreateProjectTable();

    vector<Account> AccountList;
    SelectDatabase(&AccountList);
    std::cout << "OK1\n";

    SelectWorkKeywords(&WorkKeywords);
    std::cout << "OK2\n";

    SelectImportantKeywords(&ImportantKeyworks);
    std::cout << "OK3\n";

    SelectSpamKeywords(&SpamKeyworks);
    std::cout << "OK4\n";

    SelectProjectKeywords(&ProjectKeywords);
    std::cout << "Loaded successfully!" << std::endl;
    ClearScreen;

    // Login
    setColor(LIGHTRED);
    std::cout << "Email Client\n";

    setColor(LIGHTYELLOW);
    std::cout << line;
    std::cout << "Login\n";
    std::cout << "Email: ";

    setColor(WHITE);
    std::getline(std::cin, UserEmail);
    setColor(LIGHTYELLOW);
    std::cout << "Password: ";
    setColor(WHITE);
    std::getline(std::cin, UserPassword);

    bool isLoginCompleted = false;

    for(auto account: AccountList){
        if(UserEmail == account.getEmail()){
            int tmp = 4;
            while(tmp){
                tmp--;
                if(UserPassword == account.getPassword()){
                    UserName = account.getName();
                    isLoginCompleted = true;
                    break;
                }
                else{
                    std::cout << "Wrong password! Try again: " << tmp << std::endl;
                    std::cout << "Password: ";
                    
                    std::getline(std::cin, UserPassword);
                }
            }
            if(isLoginCompleted){
                std::cout << "Completed login!" << std::endl;
                return true;
            }
            else{
                std::cerr << "Failed Login!" << std::endl;
                return false;
            }
        }
    }
    char Ans;
    std::cout << "Account does not exist" << std::endl;
    std::cout << "Register? (Y/N)" << std::endl;
    cin >> Ans;
    if(Ans != 'Y'){
        return false;
    }
    else{
        std::cout << "Your name: ";
        std::cin.ignore();
        std::getline(std::cin, UserName);

        std::cout << "Password: ";
        std::getline(std::cin, UserPassword);
        InsertDatabase(UserEmail, UserPassword, UserName, UserAutoload);
        std::cout << "Successfully registered" << std::endl;
            
        return true;
    }

    return false;
}

std::vector<std::pair<std::string, Email> > getEmailListFromFolder(std::string filter){
    std::string folderPath = "database/" + UserEmail + "/" + filter;
    std::vector<std::pair<std::string, Email> > emailList;
    std::pair<std::string, Email> tmp;
    try{
        // Lặp qua tất cả các tệp trong thư mục
        for(const auto& entry : fs::directory_iterator(folderPath)){
            // Kiểm tra xem đối tượng là một tệp tin
            if(fs::is_regular_file(entry.path())){
                // Kiểm tra xem có phải là tệp có phần mở rộng .txt không
                std::string filename  = entry.path().filename().string();
                std::string extension = entry.path().extension().string();

                
               
                    std::ifstream file (entry.path());
                    std::ostringstream file_content;
                    if (file.is_open()){
                        file_content << file.rdbuf();
                        file.close();
                    } 
                    tmp.first  = filename;
                    tmp.second = Email(file_content.str());
                    emailList.push_back(tmp);
    
            }
        }
    }
    catch (const std::exception& ex){
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return emailList;
}

void displayEmailList(std::vector<std::pair<std::string, Email> > List){
    setColor(WHITE);
    ClearScreen;
   
    cout << "Loading data..." << std::endl;

    // load data (status of Email(s))
    vector<pair<string, string> > EmailList;
    std::string filepath = "database";
    filepath = filepath + "/" + UserEmail + "/" + "EmailList.txt";
    std::ifstream ifs(filepath);
    if(ifs.is_open()){
        string id, status;
        while(!ifs.eof()){
            ifs.ignore();
            ifs >> id >> status;
            EmailList.push_back({id, status});
        }
    }
    ifs.close();
    
    cout << "Loaded successfully!" << std::endl;
    ClearScreen;

  
    // display on screen
    for(int i = 0; i < List.size(); i++){
        cout << line;
        cout << "Email #" << (i + 1) << std::endl;
        cout << "From: " << List[i].second.getSender() << std::endl;
        cout << "Subject: " << List[i].second.getSubject() << std::endl;
        
        for(int j = 0; j < EmailList.size(); j++){
            if(EmailList[j].first == List[i].first){
                cout << "Status: ";
                if(EmailList[j].second == "unread")
                    setColor(LIGHTRED);
                cout << EmailList[j].second << std::endl;
                setColor(WHITE);
                break;
            }
        }
    }
}

void displayEmail(std::pair<std::string, Email> email, int stt){
    setColor(WHITE);
    ClearScreen;

    cout << "Loading data..." << std::endl;
    
    // Mark as read
    vector<pair<string, string> > EmailList;
    std::string filepath = "database";
    filepath = filepath + "/" + UserEmail + "/" + "EmailList.txt";
    std::ifstream ifs(filepath);
    if(ifs.is_open()){
        string id, status;
        while(!ifs.eof()){
            ifs.ignore();
            ifs >> id >> status;
            EmailList.push_back({id, status});
        }
    }
    ifs.close();

    cout << "Load #1 OK" << endl;

    for(auto &tmp: EmailList){
        if(email.first.find(tmp.first) != std::string::npos){
            tmp.second = "read"; // mark
            break;
        }
    }

    cout << "Load #2 OK" << endl;

    ofstream ofs(filepath);
    for(auto tmp : EmailList){
        ofs << std::endl;
        ofs << tmp.first << " " << tmp.second;
    }
    ofs.close();

    cout << "Loaded Successfully!" << std::endl;
    ClearScreen;
    
    // display to screen
    cout << "Email #" << (stt + 1) << std::endl;
    cout << "Date: " << email.second.getDate() << std::endl;
    cout << "From: " << email.second.getSender() << std::endl;
    cout << "CC: " << email.second.getCC() << std::endl;
    cout << "Subject: " << email.second.getSubject() << std::endl;
    cout << "Content: " << email.second.getMessage() << std::endl;
    
    // Download attachment(s)
    vector<pair<string, string> > attachfile = email.second.getAttachment();
    char s;
    if(!attachfile.empty()){
        cout << "This mail has attachment(s). Do you want to download it? (Y/N)";
        cin >> s;
        if(s == 'Y')
            for(const auto& file: attachfile){
                saveFile("D:", file.second, file.first);
            }
    }
        
    cout << line << "Enter ESC to back";  
    while(true){
        s = _getch();
        if(s == ESC)
            return;
    }
}

void InboxWindow(){
    int stt;
    std::vector<std::pair<std::string, Email> > InboxList = getEmailListFromFolder("Inbox");
    

      if(InboxList.empty()){
        setColor(LIGHTRED);
        std::cout << "There are no emails in here!" << endl;
        setColor(WHITE);

        Sleep(2000);
        return;
    }

    displayEmailList(InboxList);
  
    std::cout << line;

    setColor(LIGHTAQUA);
    std::cout << "Your choice: ";
    std::cin >> stt;
    setColor(WHITE);

    if(stt == 0)
        return;
    stt--;
    displayEmail(InboxList[stt], stt);
}

void WorkMailWindow(){
    int stt;
    std::vector<std::pair<std::string, Email> > WorkList = getEmailListFromFolder("Work");
    
    if(WorkList.empty()){
        setColor(LIGHTRED);
        std::cout << "Empty!" << endl;
        setColor(WHITE);
        
        Sleep(2500);
        return;
    }
    displayEmailList(WorkList);
  
    std::cout << line;

    setColor(LIGHTAQUA);
    std::cout << "Your choice: ";
    std::cin >> stt;
    setColor(WHITE);

    if(stt == 0)
        return;

    stt--;
    displayEmail(WorkList[stt], stt);

}

void ImportantMailWindow(){
    int stt;
    std::vector<std::pair<std::string, Email> > ImportantList = getEmailListFromFolder("Important");
    
    if(ImportantList.empty()){
        setColor(LIGHTRED);
        std::cout << "Empty!" << endl;
        setColor(WHITE);
        
        Sleep(2500);
        return;
    }

    displayEmailList(ImportantList);
  
    std::cout << line;

    setColor(LIGHTAQUA);
    std::cout << "Your choice: ";
    std::cin >> stt;
    setColor(WHITE);

    if(stt == 0)
        return;
    stt--;
    displayEmail(ImportantList[stt], stt);
}

void ProjectMailWindow(){
    int stt;
    std::vector<std::pair<std::string, Email> > ProjectList = getEmailListFromFolder("Project");
    
    if(ProjectList.empty()){
        setColor(LIGHTRED);
        std::cout << "Empty!" << endl;
        setColor(WHITE);
        
        Sleep(2500);
        return;
    }

    displayEmailList(ProjectList);
  
    std::cout << line;

    setColor(LIGHTAQUA);
    std::cout << "Your choice: ";
    std::cin >> stt;
    setColor(WHITE);
    
    if(stt == 0)
        return;
    stt--;
    displayEmail(ProjectList[stt], stt);

}

void SpamMailWindow(){
    int stt;
    std::vector<std::pair<std::string, Email> > SpamList = getEmailListFromFolder("Spam");
    
    if(SpamList.empty()){
        setColor(LIGHTRED);
        std::cout << "Empty!" << endl;
        setColor(WHITE);
        
        Sleep(2500);
        return;
    }

    displayEmailList(SpamList);
  
    std::cout << line;

    setColor(LIGHTAQUA);
    std::cout << "Your choice: ";
    std::cin >> stt;
    setColor(WHITE);
    
    if(stt == 0)
        return;
    stt--;
    displayEmail(SpamList[stt], stt);

}

void MenuMailBox(){
    ClearScreen;
    std::cout << "Mailbox\n";
    std::cout << line;
    std::cout << "Enter numbers from the keyboard to select\n";
    std::cout << line;

    setColor(LIGHTGREEN);
    std::cout << "\t1. Inbox\n";
    std::cout << "\t2. Work\n";
    std::cout << "\t3. Important\n";
    std::cout << "\t4. Spam\n";
    std::cout << "\t5. Project\n";
    std::cout << "\t0. Go back\n"; 
    
    setColor(LIGHTAQUA);
    std::cout << "Your choice: ";

    int selected;
    std::cin >> selected;
    switch(selected){
        case 1:{
            InboxWindow();
            break;
        }
        case 2:{
            WorkMailWindow();
            break;
        }
        case 3:{
            ImportantMailWindow();
            break;
        }
        case 4:{
            SpamMailWindow();
            break;
        }
        case 5:{
            ProjectMailWindow();
            break;
        }
        case 0:{
            return;
        }
        default:
            break;        
    }

}

void SelectionAccountList(){
    ClearScreen;
    
    setColor(WHITE);
    std::cout << "Hi, " << UserName << std::endl;

    setColor(LIGHTYELLOW);
    std::cout << line;

    std::cout << "Enter numbers from the keyboard to select\n";
    std::cout << line;

    setColor(LIGHTGREEN);
    std::cout << "\t1. Compose mail\n";
    std::cout << "\t2. Mailbox\n";
    std::cout << "\t0. Log out\n";
    
    setColor(LIGHTAQUA);
    std::cout << "Your choice: ";

    setColor(WHITE);
}

void MailBoxWindow(){  
    SelectionAccountList();
    
    int selected;
    std::cin >> selected;
    switch (selected){
        case 1:{
            ComposeMailWindow();
            break;
        }
        case 2:{
            MenuMailBox();
            break;
        }
        case 0:{
            exit(0);
        }
        default:
            break;
    }

}


std::string OpenFileDialog(){
    OPENFILENAMEA ofn;
    char szFile[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if(GetOpenFileNameA(&ofn)){
        return std::string(szFile);
    } 
    else{ 
        return "";
    }
}

void ComposeMailWindow(){
    ClearScreen;
    
    // connect to Sever
    SMTPClient local_server("127.0.0.1", 2500);

    string sender = UserEmail, recipient, subject, body;
    string Receiver, tmp;
    vector<string> CC, BCC, AttachFile;

    cout << "Compose mail\n" << line;

    // Recipient
    cout << "To: ";
    cin.ignore();
    getline(cin, recipient);
    cout << line;
    // CC
    cout << "CC: ";
    getline(cin, Receiver);
    stringstream ssCC(Receiver);
    while (ssCC >> tmp){
        CC.push_back(tmp);
    }
    ssCC.clear();
    cout << line;
    // BCC
    cout << "BCC: ";
    getline(cin, recipient);
    stringstream ssBCC(Receiver);
    while (ssBCC >> tmp){
        BCC.push_back(tmp);
    }
    
    ssBCC.clear();
    cout << line;
    // Subject
    cout << "Subject: ";
    getline(cin, subject);

    // Body content
    cout << "Content: (please type 'end!' on a new line to finish)\n";
    std::string content;
    while(std::getline(std::cin, content) && content != "end!") {
        body += content + "\n"; 
    }
    cout << line;

    // Attach file
    cout << "Attach file: \n";
    while(tmp != ""){    
        tmp = OpenFileDialog();
        cout << tmp << endl;
        AttachFile.push_back(tmp);
    }

    cout << "Send mail? (Y/N)\n";
    getline(cin, tmp);
    if(tmp == "Y")
        cout << "Sending...\n";
    else{
        cout << "Failed to send email.\n";
        return;
    }

    // send mail
    Email email(sender, recipient, subject, body);

    for(auto R: CC)
        email.addCc(R);
    
    for(auto R: BCC)
        email.addBcc(R);

    for(auto R: AttachFile)
        email.attachFile(R);

    local_server.sendEmail(email);

    cout << "Email sent successfully!\n";
    Sleep(2500);
}

void downloadMail(){
    POP3Client local_client("127.0.0.1", 1100);
        std::string username = UserEmail;
        std::string password = UserPassword;

        local_client.login(username, password);
        local_client.loadDownloadedEmail(username);

        std::string database_folder = "database";
        std::string user_folder = database_folder + "/" + username;
        std::string file_path = user_folder + "/" + "EmailList.txt";
        std::string Inbox_folder = user_folder + "/" + "Inbox";
        std::string Work_folder = user_folder + "/" + "Work";
        std::string Important_folder = user_folder + "/" + "Important";
        std::string Spam_folder = user_folder + "/" + "Spam";
        std::string Project_folder = user_folder + "/" + "Project";

        // open or create folder
        if(!fs::exists(database_folder)){
            fs::create_directory(database_folder);
        }
        if(!fs::exists(user_folder)){
            fs::create_directory(user_folder);
        }
        if(!fs::exists(Inbox_folder)){
            fs::create_directory(Inbox_folder);
        }
        if(!fs::exists(Work_folder)){
            fs::create_directory(Work_folder);
        }
        if(!fs::exists(Important_folder)){
            fs::create_directory(Important_folder);
        }
        if(!fs::exists(Spam_folder)){
            fs::create_directory(Spam_folder);
        }
        if(!fs::exists(Project_folder)){
            fs::create_directory(Project_folder);
        }

        // load downloaded mail
        std::vector<std::string> DownloadedMail;
        std::ifstream ifs(file_path);
        if(!ifs.is_open()){
            std::ofstream fs(file_path);
            fs.close();
        }
        else{
            std::string tmp;
            while(!ifs.eof()){
                ifs.ignore();
                ifs >> tmp;
                DownloadedMail.push_back(tmp);
                ifs >> tmp;
            }
        }
        ifs.close();

        std::ofstream ofs(file_path, std::ios::app);
        if(!ofs.is_open())
            cout << "Error open file" << endl;
        // load mail on client
        std::vector<std::pair<std::string,std::string> > AccountList_email = local_client.retrieveAllEmail();

        // check and download
        for (const auto& email : AccountList_email){
            if(std::find(DownloadedMail.begin(), DownloadedMail.end(), email.second) == DownloadedMail.end()){
                string filter = filterMail(email.first);
                ofs << endl << " " << email.second << " unread";
                saveEmail(username, email.first, email.second, filter);
            }
        }
        
        ofs.close();
}

void autoDownloadMail(){   
    while(true){
        downloadMail();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

string filterMail(Email Mail){
    string content = Mail.getSender();

    for(string word: ProjectKeywords)
        if(content.find(word) != std::string::npos)
            return "Project";
            
    content += Mail.getSubject();
    content += Mail.getMessage();
    
    for(string word: WorkKeywords)
        if(content.find(word) != std::string::npos)
            return "Work";

    for(string word: ImportantKeyworks)
        if(content.find(word) != std::string::npos)
            return "Important";
    
    for(string word: SpamKeyworks)
        if(content.find(word) != std::string::npos)
            return "Spam";

    return "Inbox";
}

void setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}