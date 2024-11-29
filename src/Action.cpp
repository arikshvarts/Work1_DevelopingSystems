#include <string>
#include <vector>
#include "../include/Action.h"
enum class SettlementType;
enum class FacilityCategory;


    BaseAction :: BaseAction(){}
    ActionStatus BaseAction :: getStatus() const{
        return status; // returns 0/1 and not COMPLETED/ERROR
    }
    void BaseAction :: complete(){
        //status = 0; //need to check syntex
    }
    void BaseAction :: error(string errorMsg){
        this->errorMsg = errorMsg; //need to check if it is really field=paramater? 
    }
    const string &BaseAction :: getErrorMsg() const{
        return errorMsg;
    }




    SimulateStep :: SimulateStep(const int numOfSteps) : _numOfSteps(numOfSteps){}
    void SimulateStep :: act(Simulation &simulation){
            for(Plan p : simulation.getPlansVec()){
                p.step();
            }
        }
    const string SimulateStep :: toString() const{
         return "Simulated" + std::to_string(_numOfSteps) + " steps";
    }
    SimulateStep* SimulateStep :: clone() const{
        return new SimulateStep(*this);
        //this method return a pointer to a new SimulateStep object with the same details

        }
   

    AddPlan :: AddPlan(const string &settlementName, const string &selectionPolicy) {}
    void AddPlan :: act(Simulation &simulation){
        //livdokv lehashlim how initialize refernece
    }
    const string AddPlan :: toString() const{
        return ".";
    }
    AddPlan* AddPlan :: clone() const{
        return new AddPlan(*this);
        //this method return a pointer to a new Addplan object with the same details

    }

// class AddSettlement : public BaseAction {
//     public:
//         AddSettlement(const string &settlementName,SettlementType settlementType);
//         void act(Simulation &simulation) override;
//         AddSettlement *clone() const override;
//         const string toString() const override;
//     private:
//         const string settlementName;
//         const SettlementType settlementType;
// };



// class AddFacility : public BaseAction {
//     public:
//         AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore);
//         void act(Simulation &simulation) override;
//         AddFacility *clone() const override;
//         const string toString() const override;
//     private:
//         const string facilityName;
//         const FacilityCategory facilityCategory;
//         const int price;
//         const int lifeQualityScore;
//         const int economyScore;
//         const int environmentScore;

// };

// class PrintPlanStatus: public BaseAction {
//     public:
//         PrintPlanStatus(int planId);
//         void act(Simulation &simulation) override;
//         PrintPlanStatus *clone() const override;
//         const string toString() const override;
//     private:
//         const int planId;
// };


// class ChangePlanPolicy : public BaseAction {
//     public:
//         ChangePlanPolicy(const int planId, const string &newPolicy);
//         void act(Simulation &simulation) override;
//         ChangePlanPolicy *clone() const override;
//         const string toString() const override;
//     private:
//         const int planId;
//         const string newPolicy;
// };


// class PrintActionsLog : public BaseAction {
//     public:
//         PrintActionsLog();
//         void act(Simulation &simulation) override;
//         PrintActionsLog *clone() const override;
//         const string toString() const override;
//     private:
// };

// class Close : public BaseAction {
//     public:
//         Close();
//         void act(Simulation &simulation) override;
//         Close *clone() const override;
//         const string toString() const override;
//     private:
// };

// class BackupSimulation : public BaseAction {
//     public:
//         BackupSimulation();
//         void act(Simulation &simulation) override;
//         BackupSimulation *clone() const override;
//         const string toString() const override;
//     private:
// };


// class RestoreSimulation : public BaseAction {
//     public:
//         RestoreSimulation();
//         void act(Simulation &simulation) override;
//         RestoreSimulation *clone() const override;
//         const string toString() const override;
//     private:
// };