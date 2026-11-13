# JWT vs OAuth

**Arpan ka nichod:** aaj badi companies yahi karti hain: "Sign in with Google" daal do, wahan khola aur ho gaya. Ye easy hai, warna har
jagah alag sign up karna padta.

## Badi galatfehmi: ye ek-doosre ka option nahi

Log poochte hain "JWT lagaun ya OAuth?". Ye dono alag level ki cheezein hain:
- **JWT** = token **dikhta kaisa hai**. Ek format: signed JSON (header . payload . mohar). Payload koi bhi padh le, badla to mohar match
  nahi, 401 (Spring 03 me chala ke dekha).
- **OAuth** = token **milta kaise hai**. Poora raasta: kisi aur app ka data, bina uska password diye, istemaal karne ki ijaazat.

## Scene: "Login with Google"

Teri app pe "Login with Google" ka button. Ravi ne dabaya, Google ke page pe gaya, wahan password daala, "Allow" dabaya, aur wapas teri
app pe. Teri app ko Ravi ka Google password kabhi nahi mila. Google ne bas ek token diya: "ye Ravi hai, aur iski email padh sakte ho".
Ye OAuth hai, pass dene ka tareeka. Aur Google ka diya token aksar JWT format me hi hota hai. Isliye dono saath chalte hain, dushman nahi.

**Tera usercrud:** plain JWT hai, OAuth nahi. Login pe server khud token sign karta hai, beech me koi Google nahi.

**Compares 07 se jod:** OAuth asal me authorization hai ("is app ko access do"). "Ye Ravi hai" wala authentication uske upar ek parat
se aata hai, jiska naam **OpenID Connect** hai. To "Login with Google" = OAuth (access) + OpenID Connect (pehchaan).

## Prod wala trap

Google ka token aaya aur sirf mohar jaanchi: "haan, Google ka hi hai". Par wo token kisi aur app ke liye bana ho sakta hai. Kisi doosri
app ne Ravi ka Google token liya aur teri app pe bhej diya, to mohar sahi hai, aur tu maan lega. Isliye token ke andar `aud` (ye token
kis app ke liye bana) bhi jaancho: teri app ka client id hai ya nahi. Mohar sahi hona kaafi nahi, wahi jo authn / authz me tha.

[← COMPARES README](../README.md)
