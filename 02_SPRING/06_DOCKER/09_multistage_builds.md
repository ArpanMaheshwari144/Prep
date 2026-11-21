# Docker 09 — Multi-stage build (rasoi alag, thaali alag)

[← Back to README](00_README.md) | [← Compose](06_docker_compose.md) | [ARG vs ENV →](10_arg_vs_env.md)

---

## Koi poochhe to aise bolo

Jar banane ke liye bhaari auzaar chahiye (JDK, Maven), par jar chalane ke liye sirf JRE. Ek hi Dockerfile me do hisse
(stage): pehla bhaari auzaar se jar banata, doosra halki image leta aur pehle wale se sirf jar utha leta. Aakhri image me
sirf doosra stage jaata, pehla phenk diya jaata. Image chhoti, deploy tez, aur production me compile ke auzaar padhe nahi
rehte jinse koi chhed-chhaad kare.

---

## Kahani: rasoi aur thaali

```
Rasoi (stage 1):  chulha, chaaku, mixer, kachcha maal (JDK, Maven, source code)  -> khana bana (jar)
Thaali (stage 2): sirf plate + khana (JRE + jar)
Mehmaan ko thaali milti hai, rasoi nahi.  Final image = sirf thaali
```

---

## Chala ke dekha (usercrud me)

- Ek stage wali image (poora JDK + jar): **256 MB**
- Multi-stage (sirf JRE + jar): **154 MB**, lagbhag 40% chhoti
- Dono laptop pe bana ke `docker images` me saath dekhi.

---

## Dikkat se hal

Dikkat: har chhote code badlaav pe Maven saari dependency dobara download karta, build dheema. Hal: pehle sirf `pom.xml`
copy karke dependency utaar lo, code baad me copy karo. `pom.xml` kam badalta, isliye Docker wo hissa yaad rakhta (cache);
code badle to sirf compile dobara.

---

## Trap

**Poora folder pehle copy:** shuru me hi `COPY . .` kiya to koi bhi file badle, saari dependency phir se download.

**Root user:** dabba default me root se chalta. Production me chhota user bana ke uske naam se chalao, taaki koi ghus bhi
jaaye to haath me kam taakat ho.

---

## Aage ki kahani

Image me bahar se value kaise di jaati: build ke waqt ya chalate waqt ([10](10_arg_vs_env.md)).

[← Back to README](00_README.md) | [← Compose](06_docker_compose.md) | [ARG vs ENV →](10_arg_vs_env.md)
