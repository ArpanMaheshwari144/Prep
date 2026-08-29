package com.javatpoint.service;

import com.javatpoint.DTO.LoginDTO;
import com.javatpoint.DTO.UserDTO;
import com.javatpoint.model.LoginMessage;
import com.javatpoint.model.User;

// SERVICE INTERFACE (contract): user-related 6 kaam ke method-signatures. Logic = UserServiceImpl me.
// interface + impl alag -> loose coupling (controller interface pe depend, impl swap ho sake = SOLID 'D').
public interface UserService {

	LoginMessage addUser(UserDTO userdto);       // signup
	LoginMessage loginUser(LoginDTO logindto);   // login
	boolean updateLoginTime(String email);       // last-login timestamp update

	User findByVerificationToken(String token);  // token se user

	boolean verifyEmail(String token);           // email-verify

	boolean updatePassword(String email, String newRawPassword);  // password change

}
