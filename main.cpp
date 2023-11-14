#include "POP3Client.h"
#include "SMTPClient.h"

int main()
{

    SMTPClient local_server("127.0.0.1", 2500);

    std::string sender = "minhdang@fit.hcmus.edu.vn";
    std::string recipient = "minhdang@fit.hcmus.edu.vn";
    std::string subject = "First mail from my client";
    std::string body = "Helo";

    Email email(sender, recipient, subject, body);
    email.addBcc(sender);
    email.addCc("minhdang@gamil.com");
    email.attachFile("d:\\wallpaperflare.com_wallpaper.jpg");

    local_server.sendEmail(email);

    system("pause");

    return 0;
}