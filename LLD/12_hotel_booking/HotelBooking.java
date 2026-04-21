// =========================================================
// 🧪 SOLO LLD TEST #6 — Hotel Booking System
// Date: 2026-04-20
// Mock interview — discussion-based
// =========================================================

import java.util.*;

// ==========================
// 📋 SCOPE (from discussion)
// ==========================
// - Hotel mein rooms hain, guests check-in/check-out karte hain
// - Ek guest ek time pe sirf 1 room mein check-in kar sakta
// - Ek room ek time pe sirf 1 guest ke paas
// - Check-out pe bill = nights × pricePerNight
// - Rooms aur guests pre-populate through add methods

// ==========================
// 🏗️ CLASS DESIGN
// ==========================
//
// class Room
//   - roomId (String), type (String), pricePerNight (int), isOccupied (boolean)
//   - Provided as-is
//
// class Guest
//   - guestId (String), name (String), currentRoomId (String, null if not checked in)
//   - Provided as-is
//
// class Hotel
//   - rooms: Map<String, Room>
//   - guests: Map<String, Guest>
//
//   Methods to implement:
//   1. boolean addRoom(Room room)
//   2. boolean addGuest(Guest guest)
//   3. boolean checkIn(String guestId, String roomId)
//   4. int checkOut(String guestId, int nights)
//   5. int getAvailableRooms()

// ==========================
// 📊 REQUIREMENTS (plain English)
// ==========================
//
// ┌──────────────────────────────────────────────────────────────────────────────┐
// │ addRoom(Room room)                                                           │
// ├──────────────────────────────────────────────────────────────────────────────┤
// │ 1. If room is null → return false                                            │
// │ 2. If room with same roomId is already registered in hotel → return false    │
// │ 3. Otherwise → register the room in hotel, return true                       │
// └──────────────────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────────────────┐
// │ addGuest(Guest guest)                                                        │
// ├──────────────────────────────────────────────────────────────────────────────┤
// │ 1. If guest is null → return false                                           │
// │ 2. If guest with same guestId is already registered in hotel → return false  │
// │ 3. Otherwise → register the guest in hotel, return true                      │
// └──────────────────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────────────────┐
// │ checkIn(String guestId, String roomId)                                       │
// ├──────────────────────────────────────────────────────────────────────────────┤
// │ 1. If no guest is registered with this guestId → return false                │
// │ 2. If no room is registered with this roomId → return false                  │
// │ 3. If that room is currently occupied → return false                         │
// │ 4. If that guest is already checked into some room → return false            │
// │ 5. Otherwise:                                                                │
// │    - Mark the room occupied                                                  │
// │    - Set guest's current room to this roomId                                 │
// │    - Return true                                                             │
// └──────────────────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────────────────┐
// │ checkOut(String guestId, int nights)                                         │
// ├──────────────────────────────────────────────────────────────────────────────┤
// │ 1. If no guest is registered with this guestId → return -1                   │
// │ 2. If that guest is not currently checked into any room → return -1          │
// │ 3. If nights is zero or negative → return -1                                 │
// │ 4. Otherwise:                                                                │
// │    - Calculate bill = nights × pricePerNight of guest's current room         │
// │    - Mark that room unoccupied                                               │
// │    - Clear guest's current room                                              │
// │    - Return the bill                                                         │
// └──────────────────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────────────────┐
// │ getAvailableRooms()                                                          │
// ├──────────────────────────────────────────────────────────────────────────────┤
// │ Return the count of rooms that are currently NOT occupied                    │
// └──────────────────────────────────────────────────────────────────────────────┘

// ==========================
// 🎯 EXAMPLES
// ==========================
//
// Hotel hotel = new Hotel();
// hotel.addRoom(new Room("R1", "Single", 1000))    → true
// hotel.addRoom(new Room("R1", "Dup", 500))        → false (duplicate id)
// hotel.addGuest(new Guest("G1", "Arpan"))         → true
//
// hotel.checkIn("G1", "R1")                        → true (Arpan in R1)
// hotel.checkIn("G1", "R2")                        → false (Arpan already in R1)
// hotel.checkIn("G99", "R1")                       → false (invalid guest)
// hotel.getAvailableRooms()                        → depends on state
//
// hotel.checkOut("G1", 3)                          → 3000 (3 × 1000)
// hotel.checkOut("G1", 1)                          → -1 (not checked in)
// hotel.checkOut("G1", 0)                          → -1 (invalid nights, when checked in)

// =========================================================
// 👇 ROOM CLASS — PROVIDED AS-IS
// =========================================================

class Room {
    String roomId;
    String type;
    int pricePerNight;
    boolean isOccupied;

    public Room(String roomId, String type, int pricePerNight) {
        this.roomId = roomId;
        this.type = type;
        this.pricePerNight = pricePerNight;
        this.isOccupied = false;
    }

    public String getRoomId() {
        return roomId;
    }

    public String getType() {
        return type;
    }

    public int getPricePerNight() {
        return pricePerNight;
    }

    public boolean isOccupied() {
        return isOccupied;
    }
}

// =========================================================
// 👇 GUEST CLASS — PROVIDED AS-IS
// =========================================================

class Guest {
    String guestId;
    String name;
    String currentRoomId;

    public Guest(String guestId, String name) {
        this.guestId = guestId;
        this.name = name;
        this.currentRoomId = null;
    }

    public String getGuestId() {
        return guestId;
    }

    public String getName() {
        return name;
    }

    public String getCurrentRoomId() {
        return currentRoomId;
    }
}

// =========================================================
// 👇 HOTEL CLASS — YAHAN APNA CODE LIKH
// =========================================================

class Hotel {
    // Fields
    Map<String, Room> rooms;
    Map<String, Guest> guests;

    // Constructor
    public Hotel() {
        this.rooms = new HashMap<>();
        this.guests = new HashMap<>();
    }

    // TODO: addRoom
    public boolean addRoom(Room room) {
        // │ addRoom(Room room) │
        // ├──────────────────────────────────────────────────────────────────────────────┤
        // │ 1. If room is null → return false │
        // │ 2. If room with same roomId is already registered in hotel → return false │
        // │ 3. Otherwise → register the room in hotel, return true

        if (room == null) {
            return false;
        } else if (rooms.containsKey(room.getRoomId())) {
            return false;
        }
        rooms.putIfAbsent(room.getRoomId(), room);
        return true;
    }

    // TODO: addGuest
    public boolean addGuest(Guest guest) {
        // │ addGuest(Guest guest) │
        // ├──────────────────────────────────────────────────────────────────────────────┤
        // │ 1. If guest is null → return false │
        // │ 2. If guest with same guestId is already registered in hotel → return false
        // │
        // │ 3. Otherwise → register the guest in hotel, return true

        if (guest == null) {
            return false;
        } else if (guests.containsKey(guest.getGuestId())) {
            return false;
        }
        guests.putIfAbsent(guest.getGuestId(), guest);
        return true;

    }

    // TODO: checkIn
    public boolean checkIn(String guestId, String roomId) {
        // │ checkIn(String guestId, String roomId) │
        // ├──────────────────────────────────────────────────────────────────────────────┤
        // │ 1. If no guest is registered with this guestId → return false │
        // │ 2. If no room is registered with this roomId → return false │
        // │ 3. If that room is currently occupied → return false │
        // │ 4. If that guest is already checked into some room → return false │
        // │ 5. Otherwise: │
        // │ - Mark the room occupied │
        // │ - Set guest's current room to this roomId │
        // │ - Return true

        if (guestId == null) {
            return false;
        } else if (roomId == null) {
            return false;
        } else if (!guests.containsKey(guestId)) {
            return false;
        } else if (!rooms.containsKey(roomId)) {
            return false;
        } else if (rooms.get(roomId).isOccupied == true) {
            return false;
        } else if (guests.get(guestId).getCurrentRoomId() != null) {
            return false;
        }
        rooms.get(roomId).isOccupied = true;
        guests.get(guestId).currentRoomId = roomId;
        return true;
    }

    // TODO: checkOut (returns bill or -1)
    public int checkOut(String guestId, int nights) {
        // │ checkOut(String guestId, int nights) │
        // ├──────────────────────────────────────────────────────────────────────────────┤
        // │ 1. If no guest is registered with this guestId → return -1 │
        // │ 2. If that guest is not currently checked into any room → return -1 │
        // │ 3. If nights is zero or negative → return -1 │
        // │ 4. Otherwise: │
        // │ - Calculate bill = nights × pricePerNight of guest's current room │
        // │ - Mark that room unoccupied │
        // │ - Clear guest's current room │
        // │ - Return the bill

        if (guestId == null) {
            return -1;
        } else if (!guests.containsKey(guestId)) {
            return -1;
        } else if (guests.get(guestId).currentRoomId == null) {
            return -1;
        } else if (nights <= 0) {
            return -1;
        }

        String roomId = guests.get(guestId).currentRoomId;
        int bill = nights * rooms.get(roomId).pricePerNight;
        rooms.get(roomId).isOccupied = false;
        guests.get(guestId).currentRoomId = null;
        return bill;
    }

    // TODO: getAvailableRooms (return count)
    public int getAvailableRooms() {
        // │ getAvailableRooms() │
        // ├──────────────────────────────────────────────────────────────────────────────┤
        // │ Return the count of rooms that are currently NOT occupied
        int count = 0;
        for(Room r : rooms.values()){
            if(r.isOccupied == false){
                count++;
            }
        } 
        return count;
    }
}

// =========================================================
// 🧪 TEST RUNNER
// =========================================================

class Main {
    public static void main(String[] args) {
        Hotel hotel = new Hotel();

        // === SETUP ROOMS ===
        System.out.println("=== SETUP ROOMS ===");
        System.out.println("Add R1 Single 1000: " + hotel.addRoom(new Room("R1", "Single", 1000)));
        System.out.println("Add R2 Double 2000: " + hotel.addRoom(new Room("R2", "Double", 2000)));
        System.out.println("Add R3 Suite 5000: " + hotel.addRoom(new Room("R3", "Suite", 5000)));
        System.out.println("Add duplicate R1: " + hotel.addRoom(new Room("R1", "Dup", 100)) + " (expected false)");
        System.out.println("Add null room: " + hotel.addRoom(null) + " (expected false)");

        // === SETUP GUESTS ===
        System.out.println("\n=== SETUP GUESTS ===");
        System.out.println("Add G1 Arpan: " + hotel.addGuest(new Guest("G1", "Arpan")));
        System.out.println("Add G2 Claude: " + hotel.addGuest(new Guest("G2", "Claude")));
        System.out.println("Add G3 Gemini: " + hotel.addGuest(new Guest("G3", "Gemini")));
        System.out.println("Add duplicate G1: " + hotel.addGuest(new Guest("G1", "Dup")) + " (expected false)");
        System.out.println("Add null guest: " + hotel.addGuest(null) + " (expected false)");

        // === AVAILABLE ROOMS ===
        System.out.println("\n=== AVAILABLE BEFORE CHECK-IN ===");
        System.out.println("Available rooms: " + hotel.getAvailableRooms() + " (expected 3)");

        // === CHECK-IN ===
        System.out.println("\n=== CHECK-IN ===");
        System.out.println("Arpan checks into R1: " + hotel.checkIn("G1", "R1") + " (expected true)");
        System.out.println("  R1 isOccupied: " + hotel.rooms.get("R1").isOccupied() + " (expected true)");
        System.out.println("  Arpan currentRoom: " + hotel.guests.get("G1").getCurrentRoomId() + " (expected R1)");

        System.out.println("\nAvailable rooms now: " + hotel.getAvailableRooms() + " (expected 2)");

        System.out.println("\nArpan tries R2 (already in R1): " + hotel.checkIn("G1", "R2") + " (expected false)");
        System.out.println("Claude tries R1 (occupied): " + hotel.checkIn("G2", "R1") + " (expected false)");
        System.out.println("Invalid guest: " + hotel.checkIn("G99", "R2") + " (expected false)");
        System.out.println("Invalid room: " + hotel.checkIn("G2", "R99") + " (expected false)");

        System.out.println("\nClaude checks into R2: " + hotel.checkIn("G2", "R2") + " (expected true)");
        System.out.println("Available rooms: " + hotel.getAvailableRooms() + " (expected 1)");

        // === CHECK-OUT ===
        System.out.println("\n=== CHECK-OUT ===");
        System.out.println("Arpan checkout 3 nights: " + hotel.checkOut("G1", 3) + " (expected 3000)");
        System.out.println("  R1 isOccupied: " + hotel.rooms.get("R1").isOccupied() + " (expected false)");
        System.out.println("  Arpan currentRoom: " + hotel.guests.get("G1").getCurrentRoomId() + " (expected null)");

        System.out.println("\nAvailable rooms: " + hotel.getAvailableRooms() + " (expected 2)");

        System.out.println("\nArpan checkout again (not checked in): " + hotel.checkOut("G1", 1) + " (expected -1)");
        System.out.println("Claude checkout 0 nights: " + hotel.checkOut("G2", 0) + " (expected -1)");
        System.out.println("Claude checkout -2 nights: " + hotel.checkOut("G2", -2) + " (expected -1)");
        System.out.println("Invalid guest checkout: " + hotel.checkOut("G99", 1) + " (expected -1)");

        // === CHECK-IN AFTER CHECK-OUT ===
        System.out.println("\n=== CHECK-IN AFTER CHECK-OUT ===");
        System.out.println("Gemini checks into R1 (freed): " + hotel.checkIn("G3", "R1") + " (expected true)");
        System.out.println("Gemini checkout 1 night: " + hotel.checkOut("G3", 1) + " (expected 1000)");

        System.out.println("\nClaude checkout 5 nights from R2: " + hotel.checkOut("G2", 5) + " (expected 10000)");

        System.out.println("\nFinal available rooms: " + hotel.getAvailableRooms() + " (expected 3)");

        System.out.println("\n=== DONE ===");
    }
}
