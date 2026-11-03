# Relationship Mappings

## Kahani: kitaab ke peeche ki mohar

Library me har kitaab ke peeche ek mohar lagi hai: "lekhak = 1". Yahi FK hai, `books.author_id`. Lekhak ke register me bhi "meri kitaabein" ki
ek list hai, par wo bas ek aaina hai. Asli sach kitaab ki mohar me hai.

- Mohar jiske paas hai, wo **owning side** hai, yaani Book (`@ManyToOne @JoinColumn(name = "author_id")`).
- Lekhak ki list aaina hai, isliye wo **inverse side** hai (`@OneToMany(mappedBy = "author")`, jiska matlab hai "mohar Book ke `author` field me hai").

Agar tune sirf lekhak ki list me kitaab likh di aur kitaab pe mohar nahi lagayi, to DB me `author_id` NULL chala jaata hai, kyunki Hibernate sirf mohar padhta hai.

## Tasveer

```
authors            books (mohar = FK)
id  name           id  title   author_id
1   Arpan          1   Java    1
2   Suresh         2   Spring  1
                   3   Python  2

Student <-> Course (many-to-many) = beech ki teesri table: student_course (student_id, course_id)
```

Books table me har kitaab ke saath `author_id` ki mohar hai: Java aur Spring Arpan ki hain, Python Suresh ki. Many-to-many me, jaise Student aur Course, beech me ek teesri table banti hai, `student_course`, jisme `student_id` aur `course_id` hote hain.

## Dikkat aur ye kyun

- `mappedBy` kyun chahiye: agar ye na ho to Hibernate samajhta hai ki ye do alag rishte hain, aur ek faaltu teesri table `author_books` bana deta hai.
- Dono side set karo. Iske liye helper banao, `addBook(b) { books.add(b); b.setAuthor(this); }`, jo list me bhi daale aur mohar bhi lagaye.

## Trade-off: @ManyToMany ya junction entity

`@ManyToMany` likhna aasaan hai. Par asli kaam me beech ki table pe extra cheez chahiye hoti hai, jaise enrolledDate ya grade, aur wo `@ManyToMany` me nahi aa
sakti. Isliye `Enrollment` naam ki entity banate hain jisme do `@ManyToOne` hote hain. Code thoda zyada likhna padta hai, par poora control milta hai.

## Prod wala trap (usercrud me tha)

```java
@Entity @Data class Author { @OneToMany(mappedBy = "author") List<Book> books; }
@Entity @Data class Book   { @ManyToOne Author author; }
// log.info(author) -> Author.toString -> books -> Book.toString -> author -> ... StackOverflowError
```

`log.info(author)` ne Author ka `toString` bulaya, usne books ka, books ne apne author ka, aur ye chakkar chalta raha jab tak StackOverflowError nahi aaya. Ek line me niyam: entity pe `@Data` mat lagao. Uski jagah `@Getter @Setter` lagao, `toString` me rishte wala field bahar rakho, equals aur hashCode id pe banao, aur bahar bhejne ke liye DTO lo.
AI bhi entity pe `@Data` laga deta hai, isliye PR me ye line pakadna tera kaam hai.

## Tera project

usercrud me Book owning side hai (`@JoinColumn author_id`), aur Author inverse side hai (`mappedBy = "author"`).

## Interview line

```
Bidirectional relation me owning side wo hai jiske table me FK hai (@ManyToOne + @JoinColumn),
doosri side mappedBy se sirf aaina. mappedBy bhoolo to extra join table, sirf ek side set karo
to FK NULL, isliye helper method. Many-to-many me main junction entity leta hoon taaki beech
ki table pe extra field aa sakein.
```
