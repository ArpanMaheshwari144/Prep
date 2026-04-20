import java.util.*;

// --- ENUMS ---
// SeatType: REGULAR, PREMIUM, VIP
enum SeatType{
    REGULAR, PREMIUM, VIP;
}


// --- CLASSES ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Movie sirf film ka data (name, duration) rakhta
class Movie {
    String name, duration;

    public Movie(String name, String duration) {
        this.name = name;
        this.duration = duration;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDuration() {
        return duration;
    }

    public void setDuration(String duration) {
        this.duration = duration;
    }
}


// SRP = har class ka ek hi kaam
// Yahan kyu: Seat sirf apni state track karta (booked/available, type)
class Seat {
    String seatNumber;
    SeatType seatType;
    boolean isBooked;

    public Seat(String seatNumber, SeatType seatType, boolean isBooked) {
        this.seatNumber = seatNumber;
        this.seatType = seatType;
        this.isBooked = isBooked;
    }

    public String getSeatNumber() {
        return seatNumber;
    }

    public void setSeatNumber(String seatNumber) {
        this.seatNumber = seatNumber;
    }

    public SeatType getSeatType() {
        return seatType;
    }

    public void setSeatType(SeatType seatType) {
        this.seatType = seatType;
    }

    public boolean isBooked() {
        return isBooked;
    }

    public void setBooked(boolean booked) {
        isBooked = booked;
    }
    
}


// SRP = har class ka ek hi kaam
// Yahan kyu: Show sirf (movie + time + seats) grouping rakhta
//
// Composition = "has-a" relationship (strong ownership)
// Yahan kyu: Show HAS-A Movie aur Seats — Show delete hua toh seat mapping bhi gone
class Show{
    Movie movie;
    String time;
    List<Seat> seats;

    public Show(Movie movie, String time, List<Seat> seats) {
        this.movie = movie;
        this.time = time;
        this.seats = seats;
    }

    public Movie getMovie() {
        return movie;
    }

    public void setMovie(Movie movie) {
        this.movie = movie;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public List<Seat> getSeats() {
        return seats;
    }

    public void setSeats(List<Seat> seats) {
        this.seats = seats;
    }
}


// SRP = har class ka ek hi kaam
// Yahan kyu: Screen sirf apne shows manage karta — ek screen, multiple shows
class Screen{
    String screenNumber;
    List<Show> shows;

    public Screen(String screenNumber, List<Show> shows) {
        this.screenNumber = screenNumber;
        this.shows = shows;
    }

    public String getScreenNumber() {
        return screenNumber;
    }

    public void setScreenNumber(String screenNumber) {
        this.screenNumber = screenNumber;
    }

    public List<Show> getShows() {
        return shows;
    }

    public void setShows(List<Show> shows) {
        this.shows = shows;
    }
}


// SRP = har class ka ek hi kaam
// Yahan kyu: Cinema sirf physical theater info (name, city, screens) rakhta
class Cinema{
    String name, city;
    List<Screen> screens;

    public Cinema(String name, String city, List<Screen> screens) {
        this.name = name;
        this.city = city;
        this.screens = screens;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public List<Screen> getScreens() {
        return screens;
    }

    public void setScreens(List<Screen> screens) {
        this.screens = screens;
    }
}


// SRP = har class ka ek hi kaam
// Yahan kyu: Booking sirf confirmation record hold karta (kaun, kab, konse seat)
class Booking{
    User user;
    Show show;
    List<Seat> seats;
    String bookingTime;

    public Booking(User user, Show show, List<Seat> seats, String bookingTime) {
        this.user = user;
        this.show = show;
        this.seats = seats;
        this.bookingTime = bookingTime;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public Show getShow() {
        return show;
    }

    public void setShow(Show show) {
        this.show = show;
    }

    public List<Seat> getSeats() {
        return seats;
    }

    public void setSeats(List<Seat> seats) {
        this.seats = seats;
    }

    public String getBookingTime() {
        return bookingTime;
    }

    public void setBookingTime(String bookingTime) {
        this.bookingTime = bookingTime;
    }
}

// SRP = har class ka ek hi kaam
// Yahan kyu: BookMyShow sirf booking orchestrate karta (bookSeats, cancelBooking)
//
// Facade Pattern = complex subsystem ko simple API ke piche chhupa
// Yahan kyu: Client bookSeats() call kare, andar ka Seat check + mark booked hidden
//
// Atomic Transaction = all-or-nothing (bank transfer jaisa)
// Yahan kyu: Pehle SAB seats check — koi booked hai to null return (NO partial booking)
class BookMyShow{
    Seat seat;

    public BookMyShow(Seat seat) {
        this.seat = seat;
    }

    public Seat getSeat() {
        return seat;
    }

    public void setSeat(Seat seat) {
        this.seat = seat;
    }

    Booking bookSeats(Show show, List<Seat> selcetdSeats, User user){
        for(Seat s : selcetdSeats){
            if(s.isBooked()){
                return null;
            }
        }
        for(Seat s : selcetdSeats){
            if(!s.isBooked()){
                s.setBooked(true);
            }
        }
        return new Booking(user, show, selcetdSeats, "now");
    }

    Booking cancelBooking(Show show, List<Seat> selcetdSeats, User user){
        for(Seat s : selcetdSeats){
            if(s.isBooked()){
                s.setBooked(false);
            }
        }
        return new Booking(user, show, selcetdSeats, "user canceled");
    }
}

class User {

}


// Ticket: booking details
class Ticket{

}

// --- YAHAN APNA CODE LIKH ---




// --- END ---


class Main {
    public static void main(String[] args) {
        // Movie bana
        Movie movie = new Movie("Pushpa 2", "3hr");

        // Seats bana
        List<Seat> seats = new ArrayList<>();
        seats.add(new Seat("A1", SeatType.REGULAR, false));
        seats.add(new Seat("A2", SeatType.REGULAR, false));
        seats.add(new Seat("B1", SeatType.PREMIUM, false));
        seats.add(new Seat("C1", SeatType.VIP, false));

        // Show bana
        Show show = new Show(movie, "7:00 PM", seats);

        // Screen, Cinema
        List<Show> shows = new ArrayList<>();
        shows.add(show);
        Screen screen = new Screen("Screen 1", shows);

        // BookMyShow service
        BookMyShow bms = new BookMyShow(seats.get(0));

        // User
        User user = new User();

        // Book 2 seats
        List<Seat> selected = new ArrayList<>();
        selected.add(seats.get(0));
        selected.add(seats.get(1));
        Booking b1 = bms.bookSeats(show, selected, user);
        System.out.println("Booking: " + (b1 != null ? "SUCCESS — seats booked" : "FAILED"));

        // Dobaara same seats book — fail hona chahiye
        Booking b2 = bms.bookSeats(show, selected, user);
        System.out.println("Rebook: " + (b2 != null ? "SUCCESS" : "FAILED — already booked"));

        // Cancel
        bms.cancelBooking(show, selected, user);
        System.out.println("Cancelled!");

        // Dobaara book — ab hona chahiye
        Booking b3 = bms.bookSeats(show, selected, user);
        System.out.println("Rebook after cancel: " + (b3 != null ? "SUCCESS" : "FAILED"));

        System.out.println("BookMyShow Done!");
    }
}
