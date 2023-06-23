#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

const int MAX_QUEUE_SIZE = 5;

class Person {
public:
    virtual void AddClient(int clientId) = 0;
};

class Barber : Person {
public:
    //Реализация паттерна Синглтон.
    static Barber& GetInstance() {
        static Barber instance;  // Статический экземпляр класса
        return instance;
    }

    void CutHair(int clientId) {
        
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            condVar_.wait(lock);
            
        }
        

        int nextClientId = queue_.front();
        queue_.pop();
        std::cout << "Парикмахер начинает стричь Клиента " << nextClientId << std::endl;
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(3));  
        std::cout << "Парикмахер закончил стрижку Клиента " << nextClientId << std::endl;
        if (queue_.empty()) {
            std::cout << "Парикмахер спит." << std::endl;
        }
    }

    void AddClient(int clientId) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.size() < MAX_QUEUE_SIZE) {
            queue_.push(clientId);
            std::cout << "Клиент " << clientId << " занял очередь." << std::endl;
            condVar_.notify_one();
        }
        else {
            std::cout << "Клиент " << clientId << " увидел, что очередь полна и ушел!" << std::endl;
        }
    }

    void Stop() {
        isRunning_ = false;
        condVar_.notify_all();
    }

private:
    Barber() {}  // Приватный конструктор, чтобы предотвратить создание объектов класса Barber

    // Запрещаем копирование и присваивание
    Barber(const Barber&) = delete;
    Barber& operator=(const Barber&) = delete;
    
    bool isRunning_ = true;

    std::mutex mutex_;
    std::condition_variable condVar_;
    std::queue<int> queue_;
};

class Client {
public:
    Client(int id, Barber& barber) : id_(id), barber_(barber) {}

    void operator()() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        barber_.AddClient(id_);
    }

private:
    int id_;
    Barber& barber_;
};


void UnitTest1() {
    //Результат: 5 клиентов займут очередь. Остальные уйдут. А после этого парикмахер будет спать дальше.
    Barber& barber = Barber::GetInstance();
    std::thread barberThread([&barber]() {
        while (true) {
            barber.CutHair(0);
        }
        });

    std::thread clients[10];
    for (int i = 0; i < 10; ++i) {
        
        clients[i] = std::thread(Client(i + 1, barber));
    }

    for (int i = 0; i < 10; ++i) {
        clients[i].join();
    }
    
    barberThread.join();

    
}

void UnitTest2() {
    //Результат: 9 клиентов займут очередь. 1 уйдет. А после этого парикмахер будет спать дальше.
    //Добавляется задержка между приходом клиентов в 1 секунду.
    Barber& barber = Barber::GetInstance();
    std::thread barberThread([&barber]() {
        while (true) {
            barber.CutHair(0);
        }
        });

    std::thread clients[10];
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        clients[i] = std::thread(Client(i + 1, barber));
    }

    for (int i = 0; i < 10; ++i) {
        clients[i].join();
    }

    barberThread.join();
}

void UnitTest3() {
    //Результат: 3 клиента придут в разное время, и будут будить парикмахера.Очереди не будет. А после этого парикмахер будет спать дальше.
    //Добавляется задержка между приходом клиентов в 6 секунд.
    Barber& barber = Barber::GetInstance();
    std::thread barberThread([&barber]() {
        while (true) {
            barber.CutHair(0);
        }
        });

    std::thread clients[3];
    for (int i = 0; i < 3; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(6));
        clients[i] = std::thread(Client(i + 1, barber));
    }

    for (int i = 0; i < 3; ++i) {
        clients[i].join();
    }

    barberThread.join();
}




int main() {
    int choice = 0;
    bool repeat = true;

    setlocale(LC_ALL, "Rus");
    std::cout << "Спящий парикмахер\n";
    while (repeat) {
        std::cout << "Какой тест хотите запустить? \n1. Unit1 \n2. Unit2  \n3. Unit3 \n0. Выход." << std::endl;
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "================Тест1================\n";
            UnitTest1();
        }
        else if (choice == 2) {
            std::cout << "================Тест2================\n";
            UnitTest2();
        }
        else if (choice == 3) {
            std::cout << "================Тест3================\n";
            UnitTest3();
        }
        else if (choice == 0) {
            repeat = false;
        }
        else {
            std::cout << "Выберите из двух вариантов!" << std::endl;
        }
    }
    
   
    
    return 0;
}
