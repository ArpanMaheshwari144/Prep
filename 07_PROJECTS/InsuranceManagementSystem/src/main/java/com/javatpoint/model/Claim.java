package com.javatpoint.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;

// ENTITY -> "claim" table. Ek claim record (id + claims data).
@Entity
@Table(name = "claim")
public class Claim {
	
	    @Override
	public String toString() {
		return "Claim [id=" + id + ", claims=" + claims + "]";
	}

		public Claim() {
		super();
		// TODO Auto-generated constructor stub
	}

		public Claim(int id, String claims) {
		super();
		this.id = id;
		this.claims = claims;
	}

		public int getId() {
		return id;
	}
    @Column(name = "id")
	public void setId(int id) {
		this.id = id;
	}

	public String getClaims() {
		return claims;
	}
    
	 @Column(name = "claims")
	public void setClaims(String claims) {
		this.claims = claims;
	}

		@Id
	    @GeneratedValue(strategy = GenerationType.IDENTITY)

	    @Column(name = "id")
	    private int id;

	    @Column(name = "claims")
	    private String claims;

	   
	

}