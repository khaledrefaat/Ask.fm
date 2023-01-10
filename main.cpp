#include <iostream>
#include "vector"

using namespace std;

int userIdsIndex = 100, questionIdsIndex = 100;
int loggedInUserIndex = -1;

struct Question {
    int id, userId;
    string question;
    string answer;
    vector<Question> threads;
};

struct User {
    int id;
    string username , password;
    vector<Question> questionsToMe;
    vector<Question> questionsFromMe;
};

struct Ask {
    vector<User> users;

    void signUp() {
        cout << "Please enter username, password to complete signing up: ";
        string username, password;
        cin >> username >> password;
        bool isSignedUp = false;
        for(const auto &user : users) if(user.username == username) isSignedUp = true;
        if(isSignedUp) {
            cout << "User already exists!\n";
            return;
        }

        User newUser = {userIdsIndex++, username, password};
        users.push_back(newUser);
        loggedInUserIndex = (int) users.size() - 1;
        cout << endl;
    }
    void login() {
        cout << "Please enter username, password to login: ";
        string username, password;
        cin >> username >> password;
//        for(const auto &user : users) if(user.username == username && user.password == password) loggedInUserId = user.id, isLoggedIn = true;
        for(auto it = users.begin(); it < users.end(); ++it) {
            if(it->username == username && it->password == password) {
                loggedInUserIndex = it - users.begin();
                cout << endl;
                return;
            }
        }
        cout << "username or password are wrong\n";
    }

    void printQuestionsToMe() {
        for(const auto &question : users[loggedInUserIndex].questionsToMe) {
            cout << "Question Id(" << question.id << ") from user id(" << question.userId << ")     Question: " << question.question << "\n";
            cout << "Answer: " << question.answer << "\n";
            if(!question.threads.empty()) {
                for(const auto &thread : question.threads) {
                    cout << "Thread: Question Id(" << thread.id << ") from user id(" << thread.userId << ")     Question: " << thread.question << "\n";
                    cout << "Answer: " << thread.answer << "\n";
                }
            }
            cout << "\n";
        }
    }

    void printQuestionsFromMe() {
        for(const auto &question : users[loggedInUserIndex].questionsFromMe) {
            cout << "Question Id(" << question.id << ") from user id(" << question.userId << ")     Question: " << question.question << "\n";
            if(question.answer.empty()) cout << "Not Answered Yet.\n\n";
            else cout << "Answer: " << question.answer << "\n\n";
        }
    }

    void answerQuestion() {
        int questionId;
        string answer;
        cout << "Please enter questionId, answer: ";
        cin >> questionId >> answer;
        cout << "\n";

        int userId;
        for(auto &question : users[loggedInUserIndex].questionsToMe) {
            if(questionId == question.id) {
                question.answer = answer;
                userId = question.userId;
                break;
            }
            for(auto &thread : question.threads) {
                if(questionId == thread.id) {
                thread.answer = answer;
                userId = thread.userId;
                break;
                }
            }
        }

        for(User &user : users) {
            if(user.id == userId) {
                for (auto &question : user.questionsFromMe) {
                    if(questionId == question.id) {
                        question.answer = answer;
                        return;
                    }
                }
            }
        }
        cout << "There is no questions to you with this id!\n";
    }

    void deleteQuestion() {
        cout << "Enter the id of the question you want to delete: ";
        int questionId;
        cin >> questionId;

        int userId;
        for(auto it =  users[loggedInUserIndex].questionsToMe.begin(); it < users[loggedInUserIndex].questionsToMe.end(); it++) {
            if(it->id == questionId) {
                userId = it->userId;
                it = users[loggedInUserIndex].questionsToMe.erase(it);
                break;
            }
        }

        for(auto &user : users) {
            if(user.id == userId) {
                for(auto &question : user.questionsFromMe) {
                    if(question.id == questionId) {
                        question.answer = "";
                        return;
                    }
                }
            }
        }

        cout << "No questions with this id.";
    }

    void askQuestion() {
        int choice;
        cout << "1 - Ask new Question\n" << "2 - Continue asking on certain question";
        cin >> choice;

        if(choice == 1) {
        int userId;
        string question;
        cout << "Please enter userid, question.\n";
        cin >> userId >> question;

        for(auto &user : users) {
            if (user.id == userId) {
                Question newQuestion = {questionIdsIndex++, users[loggedInUserIndex].id, question};
                user.questionsToMe.push_back(newQuestion);
                users[loggedInUserIndex].questionsFromMe.push_back(newQuestion);
                return;
            }
        }
        cout << "No user found with this id!\n";
        }

        else if(choice == 2) {
            int userId, questionId;
            string question;
            cout << "Please enter userId, questionId, question.\n";
            cin >> userId >> questionId >> question;

            for(auto &user : users) {
                if(user.id == userId) {
                for(Question &q : user.questionsToMe) {
                    if(q.id == questionId) {
                        Question newThread = {questionIdsIndex++, users[loggedInUserIndex].id, question};
                        q.threads.push_back(newThread);
                        users[loggedInUserIndex].questionsFromMe.push_back(newThread);
                        return;
                    }
                }
                }
            }
            cout << "This user has no questions with this id!\n";
        } else cout << "Invalid choice!\n";
    }

    void listSystemUsers() {
        for(const auto &user : users) cout << user.username << " id(" << user.id << ")\n";
    }

    void feed() {
        int userId = users[loggedInUserIndex].id;

        for(const auto &user : users) {
            if(user.id != userId) {
                for(const auto &question : user.questionsToMe) {
                    cout << "Question Id(" << question.id << ") from user id(" << question.userId << ")     Question: " << question.question << "\n";
                    cout << "Answer: " << question.answer << "\n";
                    if(!question.threads.empty()) {
                        for(const auto &thread : question.threads) {
                            cout << "Thread: Question Id(" << thread.id << ") from user id(" << thread.userId << ")     Question: " << thread.question << "\n";
                            cout << "Answer: " << thread.answer << "\n";
                        }
                    }
                    cout << "\n";
                }
            }
        }
    }

    void logout() {
        loggedInUserIndex = -1;
    }

    static int menu() {
        if(loggedInUserIndex == -1) {
            cout << "1 - Login\n" << "2 - Sign Up\n";
        } else {
            cout << "1 - Print Questions To Me\n";
            cout << "2 - Print Questions From Me\n";
            cout << "3 - Answer Question\n";
            cout << "4 - Delete Question\n";
            cout << "5 - Ask Question\n";
            cout << "6 - List System Users\n";
            cout << "7 - Feed\n";
            cout << "8 - Logout\n";
        }
    }
};


int main() {
    Ask ask;
    while(true) {
        int choice;
        Ask::menu();
        cin >> choice;

        if(loggedInUserIndex == -1) {
            if(choice == 1) ask.login();
            else if(choice == 2) ask.signUp();
            else cout << "invalid Choice\n";
        } else {
            if(choice == 1) ask.printQuestionsToMe();
            else if(choice == 2) ask.printQuestionsFromMe();
            else if(choice == 3) ask.answerQuestion();
            else if(choice == 4) ask.deleteQuestion();
            else if(choice == 5) ask.askQuestion();
            else if(choice == 6) ask.listSystemUsers();
            else if(choice == 7) ask.feed();
            else if(choice == 8) ask.logout();
            else cout << "Invalid Choice\n";
        }
    }
}