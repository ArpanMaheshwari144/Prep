package com.javatpoint.DTO;

// DTO: login ke waqt client se aane wali CREDENTIALS carry karta (email + password).
// Entity nahi -> auth ke liye sirf ye 2 field chahiye, poora User row nahi. Used by: POST /login.
public class LoginDTO {
	private String email;
	private String password;

	public LoginDTO() {}   // JSON -> object (Jackson) ke liye no-arg constructor zaroori

	public LoginDTO(String email, String password) {
		this.email = email;
		this.password = password;
	}
	// getters/setters -> Jackson in fields ko JSON se bharta / padhta
	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}

}
