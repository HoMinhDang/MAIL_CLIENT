#include "Config.h"

#define ClearScreen system("cls")
#define line "--------------------\n"

/* Color */
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTAQUA 11
#define LIGHTRED 12
#define LIGHTPURPLE 13
#define LIGHTYELLOW 14
#define BRIGHTWHITE 15

/* Mail box */
#define LOGINWINDOW 0
#define MAILBOXWINDOW 1
#define COMPOSEMAILWINDOW 2
#define INBOXWINDOW 3
#define WORKMAILWINDOW 4
#define IMPORTANTMAILWINDOW 5
#define SPAMMAILWINDOW 6

/* Flter */
#define INBOX 0
#define IMPORTANT 1
#define WORK 2
#define SPAM 3
#define PROJECT 4


#define RETURN 27 // ALT or ESC
#define SUMIT -32 // CTRL

void setColor(int);

string OpenFileDialog();

bool LoginWindow();

string filterMail(Email);


// download mail
void downloadMail();

void autoDownloadMail();

// display
void InboxWindow();

void WorkMailWindow();

void ImportantMailWindow();

void SpamMailWindow();

void ProjectMailWindow();

void ComposeMailWindow();

void SelectionList();

void MenuMailBox();

void MailBoxWindow();


