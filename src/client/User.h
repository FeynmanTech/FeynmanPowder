#ifndef USER_H_
#define USER_H_

#include <string>


class User
{
public:
	enum Elevation
	{
		ElevationAdmin, ElevationModerator, ElevationNone
	};
	int ID;
	std::string Username;
	std::string SessionID;
	std::string SessionKey;

	int IDNet;
	std::string UsernameNet;
	std::string SessionIDNet;
	std::string SessionKeyNet;

	int IDOff;
	std::string UsernameOff;
	std::string SessionIDOff;
	std::string SessionKeyOff;

	std::string PasswordNet;
	std::string PasswordOff;

	Elevation UserElevation;
	User(int id, std::string username):
		ID(id),
		Username(username),
		SessionID(""),
		SessionKey(""),
		UserElevation(ElevationNone)
	{

	}
};


#endif /* USER_H_ */
