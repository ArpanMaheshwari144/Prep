package com.arpan.orderservice;

import jakarta.persistence.*;

@Entity
@Table(name = "orders")   // "order" SQL reserved word -> table naam "orders"
public class Order {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String item;
    private double amount;
    private String status;   // "CREATED" -> baad me "PAID"

    public Order() {}
    public Order(String item, double amount, String status) {
        this.item = item;
        this.amount = amount;
        this.status = status;
    }

    public Long getId() { return id; }
    public void setId(Long id) { this.id = id; }

    public String getItem() { return item; }
    public void setItem(String item) { this.item = item; }

    public double getAmount() { return amount; }
    public void setAmount(double amount) { this.amount = amount; }

    public String getStatus() { return status; }
    public void setStatus(String status) { this.status = status; }
}
