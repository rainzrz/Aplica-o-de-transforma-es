# PGCCHIB - Processamento Gráfico

Este repositório é baseado na estrutura do projeto PGCCHIB - Processamento Gráfico, fornecido pela disciplina de Processamento Gráfico.

A atividade vivencial do dia 20 de maio foi implementada nos arquivos `exercicio1.cpp`, `exercicio2.cpp` e `exercicio3.cpp` localizado em `src/Vivencial`.

## exercicio1.cpp
![image](https://github.com/user-attachments/assets/10233c7f-6c5c-4227-8613-c0c4a5057f31)

## exercicio2.cpp
![image](https://github.com/user-attachments/assets/0c192032-23c2-426b-8e8a-5f0de4192d76)

## exercicio3.cpp
![image](https://github.com/user-attachments/assets/ebea1509-d1b1-4467-9161-53bcd7e4c05c)

## 🛠️ Como compilar e rodar o projeto (PowerShell - Windows)
Se quiser compilar e rodar o projeto do zero, siga os comandos abaixo:
```powershell
cd ~seudiretório\V1_ProcessamentoGrafico\
Remove-Item -Recurse -Force .\build\
mkdir build
cd build
cmake ..
cmake --build .
.\Debug\Triangulos.exe
```

Siga as instruções detalhadas em [GettingStarted.md](GettingStarted.md) para configurar e compilar o projeto.

## ⚠️ **IMPORTANTE: Baixar a GLAD Manualmente**
Para que o projeto funcione corretamente, é necessário **baixar a GLAD manualmente** utilizando o **GLAD Generator**.

### 🔗 **Acesse o web service do GLAD**:
👉 [GLAD Generator](https://glad.dav1d.de/)

### ⚙️ **Configuração necessária:**
- **API:** OpenGL  
- **Version:** 3.3+ (ou superior compatível com sua máquina)  
- **Profile:** Core  
- **Language:** C/C++  

### 📥 **Baixe e extraia os arquivos:**
Após a geração, extraia os arquivos baixados e coloque-os nos diretórios correspondentes:
- Copie **glad.h** para `include/glad/`
- Copie **khrplatform.h** para `include/glad/KHR/`
- Copie **glad.c** para `common/`

🚨 **Sem esses arquivos, a compilação falhará!** É necessário colocá-los nos diretórios corretos conforme a orientação acima.

---



