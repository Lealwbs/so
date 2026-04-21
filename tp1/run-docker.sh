#!/bin/bash

#---------------------------#
# Script para criar imagem, iniciar e acessar o container do tp
#---------------------------#

# Cores para output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

IMAGE_NAME="tp1-xv6"
CONTAINER_NAME="tp1-xv6-container"

echo -e "${BLUE}=== XV6 RISC-V Docker Setup ===${NC}\n"

# Verificar se a imagem já existe
if docker image inspect $IMAGE_NAME >/dev/null 2>&1; then
    echo -e "${GREEN}✓ Imagem '$IMAGE_NAME' já existe${NC}"
else
    echo -e "${YELLOW}→ Criando imagem '$IMAGE_NAME'...${NC}"
    docker build -t $IMAGE_NAME .
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Imagem criada com sucesso${NC}\n"
    else
        echo -e "${RED}✗ Erro ao criar a imagem${NC}"
        exit 1
    fi
fi

# Verificar se o container já existe
if docker ps -a --format '{{.Names}}' | grep -q "^${CONTAINER_NAME}$"; then
    echo -e "${YELLOW}→ Container '$CONTAINER_NAME' já existe${NC}"
    echo -e "${BLUE}Iniciando container...${NC}"
    docker start $CONTAINER_NAME
    docker exec -it $CONTAINER_NAME /bin/bash
else
    echo -e "${YELLOW}→ Criando novo container '$CONTAINER_NAME'...${NC}"
    docker run -it \
        -v $(pwd):/tp1-sistemas-operacionais \
        -w /tp1-sistemas-operacionais \
        --name $CONTAINER_NAME $IMAGE_NAME
fi

echo -e "\n${GREEN}✓ Pronto!${NC}"
