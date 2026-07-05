package com.arpan.paymentservice;

// PayRequest — POST /pay ka request body { orderId, amount } (DTO).
public class PayRequest {
    private String orderId;
    private double amount;

    public String getOrderId() { return orderId; }
    public void setOrderId(String orderId) { this.orderId = orderId; }

    public double getAmount() { return amount; }
    public void setAmount(double amount) { this.amount = amount; }
}
