#include <iostream>
#include <set>
#include <thread>
#include <vector>

#include "Account.h"
#include "TransactionManager.h"

using namespace std;

int main() {
    auto& manager = TransactionManager::getInstance();

    Account account("A1", 1000);

    vector<thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() {
            manager.deposit(&account, 100);
            manager.withdraw(&account, 50);
        });
    }

    for (auto& t : threads) t.join();

    cout << "Balance final: "
              << account.getBalance()
              << endl;

    cout << "Direccion manager: "
              << &TransactionManager::getInstance()
              << endl;

    // ---- Verificaciones adicionales ----

    // 1) Todas las operaciones quedaron registradas (10 depositos + 10 retiros).
    cout << "Transacciones registradas: "
              << manager.getTransactionCount() << endl;

    // 2) La instancia es unica aunque se pida desde muchos hilos a la vez.
    vector<const TransactionManager*> addresses(20);
    vector<thread> checkers;
    for (int i = 0; i < 20; ++i) {
        checkers.emplace_back([&addresses, i]() {
            addresses[i] = &TransactionManager::getInstance();
        });
    }
    for (auto& t : checkers) t.join();
    set<const TransactionManager*> unique(addresses.begin(), addresses.end());
    cout << "Direcciones distintas obtenidas desde 20 hilos: "
              << unique.size() << endl;

    // 3) Estres: 100 hilos x 1000 retiros de 1 sobre una cuenta con 50000.
    //    Sin condiciones de carrera el saldo debe quedar exactamente en 0
    //    y nunca puede volverse negativo.
    Account stress("B1", 50000);
    vector<thread> workers;
    for (int i = 0; i < 100; ++i) {
        workers.emplace_back([&]() {
            for (int j = 0; j < 1000; ++j) manager.withdraw(&stress, 1);
        });
    }
    for (auto& t : workers) t.join();
    cout << "Prueba de estres (esperado 0): "
              << stress.getBalance() << endl;

    return 0;
}
