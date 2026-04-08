#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>

/**
 * @namespace itandroids_vss
 * Encapsula as classes para evitar conflitos de nomes.
 */
namespace itandroids_vss {

    /**
     * @class Robo
     * Classe base abstrata para os robôs do time.
     */
    class Robo {
    protected:
        std::string nome;
        int numero;

    public:
        Robo(const std::string& n, int num) : nome(n) {
            if (num < 1 || num > 100) {
                throw std::invalid_argument("O número do robô deve estar entre 1 e 100.");
            }
            numero = num;
        }

        virtual ~Robo() = default;

        // Getters constantes para acesso seguro
        std::string getNome() const { return nome; }
        int getNumero() const { return numero; }

        /**
         * @brief Método virtual para resetar estatísticas da classe.
         */
        virtual void reset() = 0;
    };

    class Goleiro : public Robo {
    private:
        int golsSofridos = 0;

    public:
        using Robo::Robo; // Herda construtores

        int getGolsSofridos() const { return golsSofridos; }
        void registrarGolSofrido() { golsSofridos++; }
        
        void reset() override { golsSofridos = 0; }
    };

    class Zagueiro : public Robo {
    private:
        int faltasCometidas = 0;

    public:
        using Robo::Robo;

        int getFaltas() const { return faltasCometidas; }
        void registrarFalta() { faltasCometidas++; }
        
        void reset() override { faltasCometidas = 0; }
    };

    class Atacante : public Robo {
    private:
        int golsMarcados = 0;

    public:
        using Robo::Robo;

        int getGolsMarcados() const { return golsMarcados; }
        void registrarGol() { golsMarcados++; }
        
        void reset() override { golsMarcados = 0; }
    };

    /**
     * @class Time
     * Gerencia a composição do time e saldo de gols.
     * Utiliza std::unique_ptr para garantir que a memória seja liberada automaticamente.
     */
    class Time {
    private:
        std::string nome;
        int saldoGols = 0;
        std::unique_ptr<Goleiro> goleiro;
        std::vector<std::unique_ptr<Robo>> jogadoresLinha;

    public:
        explicit Time(const std::string& n) : nome(n) {}

        std::string getNome() const { return nome; }
        int getSaldoGols() const { return saldoGols; }
        void atualizarSaldo(int novoSaldo) { saldoGols = novoSaldo; }

        void setGoleiro(std::unique_ptr<Goleiro> g) {
            goleiro = std::move(g);
        }

        void adicionarJogador(std::unique_ptr<Robo> j) {
            if (jogadoresLinha.size() >= 10) {
                throw std::runtime_error("Limite de 10 jogadores de linha atingido.");
            }
            jogadoresLinha.push_back(std::move(j));
        }

        // Acesso seguro aos jogadores
        const Robo* getJogador(size_t index) const {
            if (index >= jogadoresLinha.size()) return nullptr;
            return jogadoresLinha[index].get();
        }

        const Goleiro* getGoleiro() const { return goleiro.get(); }
    };
}

int main() {
    using namespace itandroids_vss;

    // Criar um time
    Time time("Meu Time");

    // Criar jogadores
    auto goleiro = std::make_unique<Goleiro>("Goleiro1", 1);
    auto zagueiro = std::make_unique<Zagueiro>("Zagueiro1", 2);
    auto atacante = std::make_unique<Atacante>("Atacante1", 3);

    // Adicionar ao time
    time.setGoleiro(std::move(goleiro));
    time.adicionarJogador(std::move(zagueiro));
    time.adicionarJogador(std::move(atacante));

    // Testar algumas funções
    std::cout << "Nome do time: " << time.getNome() << std::endl;
    if (time.getGoleiro()) {
        std::cout << "Goleiro: " << time.getGoleiro()->getNome() << std::endl;
    }

    return 0;
}