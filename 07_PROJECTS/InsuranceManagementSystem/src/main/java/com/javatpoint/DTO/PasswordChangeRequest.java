package com.javatpoint.DTO;

// DTO: password-change form ka data (email, currentPassword, newPassword, confirmPassword).
// confirmPassword sirf VALIDATION ke liye (new == confirm?) -> DB me kabhi store nahi hota. Used by: POST /change/password.
public class PasswordChangeRequest {

	private String email;
	private String currentPassword;   // purana (verify karne ko)
	private String newPassword;       // naya (set karne ko)
	private String confirmPassword;   // sirf match-check, store nahi

	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
	}
	public String getCurrentPassword() {
		return currentPassword;
	}
	public void setCurrentPassword(String currentPassword) {
		this.currentPassword = currentPassword;
	}
	public String getNewPassword() {
		return newPassword;
	}
	public void setNewPassword(String newPassword) {
		this.newPassword = newPassword;
	}
	public String getConfirmPassword() {
		return confirmPassword;
	}
	public void setConfirmPassword(String confirmPassword) {
		this.confirmPassword = confirmPassword;
	}

}
