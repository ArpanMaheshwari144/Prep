# LOCUST (STEP 2) — nakli users jo baar-baar API ko request maarte.
# Web-dashboard (localhost:8089) me LIVE dikhta: RPS, Response-time (latency), Users.
# Chalao: python -m locust -f locustfile.py --host http://localhost:3000
from locust import HttpUser, task, between

class ApiUser(HttpUser):
    wait_time = between(0.1, 0.5)     # har nakli user 2 request ke beech 0.1-0.5s rukta

    @task
    def get_users(self):
        self.client.get("/api/users")
