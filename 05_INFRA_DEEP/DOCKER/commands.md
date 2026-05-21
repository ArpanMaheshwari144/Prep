# Docker Commands Reference

## IMAGES
docker pull <image>                  - Pull image from registry
docker images                        - List local images
docker build -t <name> .             - Build image from Dockerfile
docker rmi <image>                   - Remove image


## CONTAINER LIFECYCLE
docker run -d -p H:C --name N IMG    - Start container detached, port mapped
docker ps                            - List running containers
docker ps -a                         - List all (including stopped)
docker start <name>                  - Start existing stopped container
docker stop <name>                   - Stop running container
docker rm <name>                     - Delete container
docker exec -it <name> bash          - Open shell inside container
docker exec <name> <cmd>             - Run one command inside container
docker logs <name>                   - Show container logs
docker logs -f <name>                - Follow logs (live tail)


## DOCKER COMPOSE
docker compose up                    - Start all services (foreground)
docker compose up -d                 - Start all (background)
docker compose up --build            - Rebuild images then start
docker compose down                  - Stop + remove containers (volumes safe)
docker compose down -v               - Stop + remove containers + VOLUMES
docker compose ps                    - List services in this compose
docker compose logs                  - All services logs
docker compose logs <service>        - Specific service logs


## VOLUMES
docker volume ls                     - List volumes
docker volume rm <name>              - Delete volume
docker volume inspect <name>         - Show volume details


## NETWORK
docker network ls                    - List networks
docker network inspect <name>        - Show network details


## CLEANUP
docker system prune                  - Remove stopped containers + unused
docker system prune -a               - Aggressive (all unused images too)
