# HELM — Kubernetes ka Package Manager

> Helm = K8s ke liye wahi jo npm (JS), maven (Java), apt (Linux) — **package manager**.
> K8s ki bikhri YAMLs ko reusable **Chart** me bundle karta + `values.yaml` se config inject.

---

## PROBLEM jo ye solve karta
todoapp ke `k8s/` folder me kai YAML the — Deployment, Service, mysql, redis, ConfigMap.
Ab har environment ke liye alag values chahiye:
- **dev**  : replicas=1, image=todoapp:dev
- **prod** : replicas=5, image=todoapp:v2, zyada memory

Bina Helm -> har env ke liye saari YAML **copy-paste + values badlo** -> messy, galti ka chance.

## FIX — Chart + blanks + values
Saari YAML ek **Chart** me bundle. Hardcode ki jagah **blanks**:
```yaml
# templates/deployment.yaml  (Chart ke andar)
replicas: {{ .Values.replicas }}      # blank -- hardcode nahi
image:    {{ .Values.image }}
```
Ek **`values.yaml`** (= "helm config") wo blanks bharti:
```yaml
# values.yaml -- dev
replicas: 1
image: todoapp:dev
```
=> **ek Chart -> saare environment**, bas values.yaml swap karo.

## VISUAL
```
  bikhri YAMLs          HELM CHART                values.yaml           deploy
  (Deployment,     -->  (templates +        -->  (dev/prod       -->   (ek command me
   Service, ...)         {{ .Values.x }})         alag values)          poora app)
```

## 3 TUKDE (yaad rakh)
| Tukda            | Kya hai |
|------------------|---------|
| **Chart**        | Pura package (templates + default values + Chart.yaml metadata) |
| **templates/**   | YAML with `{{ .Values.* }}` blanks |
| **values.yaml**  | Blanks bharne wali config (← "helm config") |

Chart structure:
```
mychart/
  Chart.yaml        # naam, version (metadata)
  values.yaml       # default config (blanks ki values)
  templates/        # YAML with {{ .Values.* }}
    deployment.yaml
    service.yaml
```

## COMMANDS (ek command me pura app)
```
helm install todoapp ./mychart          # saari YAML ek saath deploy
helm install todoapp ./mychart -f prod-values.yaml   # prod config se
helm upgrade todoapp ./mychart          # nayi version deploy
helm rollback todoapp 1                 # purani revision pe wapas (BADA faayda)
helm list                               # kya-kya installed
helm uninstall todoapp                  # sab hata do
```
- Ek installed chart = ek **"release"** (todoapp = release naam).
- Har upgrade ek **revision** banata -> rollback easy (versioned deployments).

## KYUN kaam ka (real-dev / JP)
- **DRY**: ek chart, N environment (values swap).
- **Versioned deploy + 1-command rollback** (kuch toota -> `helm rollback` -> pehle wala state).
- **Public charts**: mysql/redis/nginx ready-made charts (`helm install` -> ban gaya), scratch se YAML nahi likhni.

## POWER LINE
> "Helm K8s ka package manager hai — manifests ko reusable charts me bundle karta hai,
>  values.yaml se environment-specific config inject hoti hai, aur versioned install/upgrade/
>  rollback deta hai. DRY deployments cross-environment."

---
> Pattern wahi: naya dikha -> samjha -> ab tera. (Docker -> K8s -> Helm = deployment ki agli seedhi.)
