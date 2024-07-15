#include <iostream>
#include <vector>
using namespace std;
//Polymorphism 2
//HomeWork 2 #1  
class Invoice;
//Validations (extension for the payroll company)


class Payable {
public:
    virtual int GetTotalPayed() 
    {
        cout << "\nNothing here!\n";
        return -1;
    }
    virtual ~Payable() {}
};

class Worker
{
private:
    string name, email, national_id;
public:
    Worker(string name, string email, string national_id) : name(name), email(email), national_id(national_id) {}
    virtual ~Worker() {}
};
class Employee :public Worker, public Payable
{
protected:
    int total_payed;
public:
    virtual void PayMoney() { cout << "\nNothing here\n"; };
    Employee(string name, string email, string national_id, int total_payed) :Worker(name, email, national_id), total_payed(total_payed) {}
    int GetTotalPayed()  override
    {
        return total_payed;
    }
    virtual ~Employee() {}
};

class HourlyBasedEmployee : public Employee {
private:
    int hour_rate, hours_worked;
public:
    HourlyBasedEmployee(string name, string email, string national_id, int hour_rate, int hours_worked, int total_payed = 0) :
        Employee(name, email, national_id, total_payed), hour_rate(hour_rate), hours_worked(hours_worked) {}
    virtual void PayMoney() override
    {
        total_payed = hour_rate * hours_worked;
    }
    virtual int GetTotalPayed() const
    {
        return  hour_rate * hours_worked;
    }
    //virtual ~HourlyBasedEmployee() {}

};

class SalaryBasedEmployee : public Employee {
private:
    int salary;
public:
    SalaryBasedEmployee(string name, string email, string national_id, int salary, int total_payed = 0) :
        Employee(name, email, national_id, total_payed), salary(salary) {}
    virtual void PayMoney() override
    {
        total_payed = salary;
    }
    virtual int GetTotalPayed() override
    {
        return salary;
    }
    int GetSalary() const
    {
        return salary;
    }
    virtual ~SalaryBasedEmployee() {}
};

class CommisionSalaryBasedEmployee : public SalaryBasedEmployee {
private:
    int extra_ciel = 1000; double sales_ratio;
public:
    CommisionSalaryBasedEmployee(string name, string email, string national_id, int salary, double sales_ratio, int total_payed = 0) :
        SalaryBasedEmployee(name, email, national_id, salary, total_payed = 0), sales_ratio(sales_ratio) {}
    double GetBonus() const
    {
        return sales_ratio * extra_ciel;
    }
    virtual void PayMoney() override
    {
        total_payed = GetSalary() + GetBonus();
    }
    virtual int GetTotalPayed() override
    {
        return  GetSalary() + GetBonus();
    }
    // virtual ~CommisionSalaryBasedEmployee() {}
};

class Volunteer : Worker, public Payable {
public:
    Volunteer(string name, string email, string national_id) :Worker(name, email, national_id)
    {

    }
    virtual int GetTotalPayed()  override
    {
        return 15;
    }
}; //don't know what to implement



class Item {
protected:
    double item_price;
    string item_name;
public:
    Item(double item_price, string item_name) :
        item_price(item_price), item_name(item_name) {}
    string GetItemName() const
    {
        return item_name;
    }
    double GetItemPrice() const
    {
        return item_price;
    }
    virtual Item* Clone()
    {
        throw new logic_error("Not implemented");
    }

};

class Book : public Item {
private:
    string book_author_name;
public:
    Book(double item_price, string item_name, string book_author_name) : Item(item_price, item_name), book_author_name(book_author_name) {}
    string GetBookAuthorName() const
    {
        return book_author_name;
    }
    virtual Item* Clone() override
    {
        return new Book(*this);
    }
};

class Food : public Item {
private:
    string color;
public:
    Food(double item_price, string item_name, string color)
        : Item(item_price, item_name), color(color) {}
    string GetColor() const
    {
        return color;
    }
    virtual Item* Clone() override
    {
        return new Food(*this);
    }
};
class ValidationRule
{
public:
    virtual bool ValidateInvoice(Invoice* inv) = 0;
    virtual  ValidationRule* Clone() = 0;
    virtual ~ValidationRule() {};
};
class ValidationGroup
{
protected:
    vector<ValidationRule*> rules;
    virtual void LoadRules() = 0; //load dummy data 
    virtual void FreeMemory() = 0; //frees the rules memory
public:
    virtual void AddRule(ValidationRule* rule) = 0; //add rule to the group
    virtual ValidationRule* GetRule(int pos) = 0; //get a specific rule from the group
    virtual bool TestRules(Invoice* inv) = 0;//test all rules , returns true if all passed
    virtual ~ValidationGroup() {};

};
class Invoice : public Payable {
private:
    vector<Item*> items;
   ValidationGroup* v;
public:
   Invoice( ValidationGroup* VR) : v(VR) {}
    void AddItem(Item* item)
    {
        items.push_back(item->Clone());
    }
    Item* GetItem(int pos)
    {
        return items[pos];
    }
    virtual int GetTotalPayed()  override {
        cout << "Invoice\n";
        v->TestRules(this);
        double sum = 0;
        for (const Item* item_ptr : items)
            sum += item_ptr->GetItemPrice();
        return sum;
    }
};

class VR1 : public ValidationRule {
    virtual bool ValidateInvoice( Invoice* inv) override
    {
        
            cout << "\nPassed VR1\n";
            return true;
        
       // cout << "\nFailed VR1\n";
        
    }
    virtual ValidationRule* Clone() override
    {
        return new VR1(*this);
    }
};
class VR2 : public ValidationRule {

    virtual bool ValidateInvoice( Invoice* inv) override
    {
        cout << "\nPassed VR2\n";
        return true;
    }
    virtual ValidationRule* Clone() override
    {
        return new VR2(*this);
    }
};
class VR3 : public ValidationRule {
    virtual bool ValidateInvoice(Invoice* inv) override
    {
        //if (inv->GetTotalPayed() < 5)
        
            cout << "\nFailed VR3\n";
            return false;
        
        cout << "\nPassed VR3\n";
        return true;
    }
    virtual ValidationRule* Clone() override
    {
        return new VR3(*this);
    }
};

class MandatoryValidationGroup : public ValidationGroup {
protected:
    virtual void LoadRules()
    {
        ValidationRule* new_rule = new VR1();
        rules.push_back(new_rule);
        new_rule = new VR2();
        rules.push_back(new_rule);
        new_rule = new VR3();
        rules.push_back(new_rule);
    }
    virtual void FreeMemory() {
        for (auto& rule : rules)
        {
            delete rule;
            rule = nullptr;
        }
        rules.clear();
    }
public:
    virtual void AddRule(ValidationRule* rule)
    {
        rules.push_back(rule->Clone());
    }
    virtual ValidationRule* GetRule(int pos)
    {
        if (pos > 0 && pos < rules.size())
            return rules[pos];
        else
            return nullptr;
    }
    virtual bool TestRules( Invoice* inv)
    {
        for (auto& rule : rules)
        {
            if (rule->ValidateInvoice(inv))
                continue;
            return false;                //if passed all tests it won't come here, if came here, means that a test has failed
        }
        return true;
    }
    MandatoryValidationGroup()
    {
        LoadRules();
    }
    virtual ~MandatoryValidationGroup()
    {
        FreeMemory();
    }
};

class Payroll {
private:
    vector<Payable*> payables;

public:
    void AddPayable(Payable* payable) {
        payables.push_back(payable);	// we should add copy
    }
    void Pay() {
        cout << "Payroll::Pay\n";
        ;
        for (const auto payable : payables)
            cout << payable->GetTotalPayed() << "\n";
    }
};

class Company {
private:
    Payroll payroll;
public:
    Company() {
    }

    void Run() {
        payroll.AddPayable(new Volunteer("mohamed", "mohamed@gmail.com", "845"));
        payroll.AddPayable(new HourlyBasedEmployee("ahmed", "ahmed@gmail.com", "4547", 12, 5));
        payroll.AddPayable(new SalaryBasedEmployee("mahmoud", "m@gmail.com", "4878455", 2000));
        payroll.AddPayable(new CommisionSalaryBasedEmployee("mostafa", "mos@gmail.com", "54878", 2000, .75));
        ValidationGroup* vMandatroy = new MandatoryValidationGroup();
        Invoice* invoice = new Invoice(vMandatroy);
        invoice->AddItem(new Book(300, "Book", "Mohamed_Emad"));
        invoice->AddItem(new Food(15, "Apple", "red"));
      //  ValidationGroup* vMandatroy = new MandatoryValidationGroup();
       // cout << "\nThe result of mandatroy validation is: " << vMandatroy->TestRules(invoice) << endl;
        payroll.AddPayable(invoice);

        payroll.Pay();
        delete vMandatroy;
    }
    ~Company()
    {
       //deelete the vMandatroy
    }
};

int main() {
    Company company;
    company.Run();

    return 0;
