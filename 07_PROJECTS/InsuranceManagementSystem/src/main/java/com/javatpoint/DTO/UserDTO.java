package com.javatpoint.DTO;

// DTO: signup/registration ka data carry karta (userid, username, email, password).
// Client ye plain fields bhejta -> service ise User ENTITY me map karta (password HASH karke, token set karke DB me).
// DTO = "kachcha input"; Entity = "DB-ready". Used by: POST /save (addUser).
public class UserDTO {

	private int userid;
	private String username;
	private String password;   // plain yahan; DB me jaane se pehle BCrypt-hash hota
	private String email;

	public UserDTO() {}

	public UserDTO(int userid, String username, String email, String password) {
		this.userid = userid;
		this.username = username;
		this.password = password;
		this.email = email;
	}
	public int getUserid() {
		return userid;
	}
	public void setUserid(int userid) {
		this.userid = userid;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
	}

}
