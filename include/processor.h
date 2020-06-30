#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    float m_PrevTotal = 0.0;
    float m_PrevIdle= 0.0;
};

#endif