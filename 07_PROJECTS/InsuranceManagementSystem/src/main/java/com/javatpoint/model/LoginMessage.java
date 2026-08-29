package com.javatpoint.model;

// POJO (entity NAHI). API RESPONSE wrapper: {message, status} -> login/save ka result client ko batata.
public class LoginMessage {
	String message;
	Boolean status;
	public LoginMessage() {
		super();
		// TODO Auto-generated constructor stub
	}
	public LoginMessage(String message, Boolean status) {
		super();
		this.message = message;
		this.status = status;
	}
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		this.message = message;
	}
	public Boolean getStatus() {
		return status;
	}
	public void setStatus(Boolean status) {
		this.status = status;
	}
	

}
