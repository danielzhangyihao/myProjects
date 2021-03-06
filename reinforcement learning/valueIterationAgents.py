# valueIterationAgents.py
# -----------------------
# Licensing Information:  You are free to use or extend these projects for 
# educational purposes provided that (1) you do not distribute or publish 
# solutions, (2) you retain this notice, and (3) you provide clear 
# attribution to UC Berkeley, including a link to 
# http://inst.eecs.berkeley.edu/~cs188/pacman/pacman.html
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero 
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and 
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


import mdp, util

from learningAgents import ValueEstimationAgent

class ValueIterationAgent(ValueEstimationAgent):
    """
        * Please read learningAgents.py before reading this.*

        A ValueIterationAgent takes a Markov decision process
        (see mdp.py) on initialization and runs value iteration
        for a given number of iterations using the supplied
        discount factor.
    """
    def __init__(self, mdp, discount = 0.9, iterations = 10):
        """
          Your value iteration agent should take an mdp on
          construction, run the indicated number of iterations
          and then act according to the resulting policy.

          Some useful mdp methods you will use:
              mdp.getStates()
              mdp.getPossibleActions(state)
              mdp.getTransitionStatesAndProbs(state, action)
              mdp.getReward(state, action, nextState)
              mdp.isTerminal(state)
        """
        self.mdp = mdp
        self.discount = discount
        self.iterations = iterations
        self.values = util.Counter() # A Counter is a dict with default 0

        # Write value iteration code here
        "*** YOUR CODE HERE ***"
        states=mdp.getStates()
        for element in states:
          self.values[element]=0
        index=0
        previousValue=util.Counter()
        while index<iterations:
          previousValue=self.values.copy() 
          for s in states:
            
            possibleActions=mdp.getPossibleActions(s)
            

            if len(possibleActions)!=0:
              IterValues=[]
              for stateAction in possibleActions:
                probs=util.Counter()
                s_value=util.Counter()
                reward=util.Counter()
                StatesAndProbs=self.mdp.getTransitionStatesAndProbs(s,stateAction)
                
                for sPrime in StatesAndProbs:
                  
                  if len(StatesAndProbs)!=0:
                    probs[sPrime[0]]=sPrime[1]
                    s_value[sPrime[0]]=self.discount*(previousValue[sPrime[0]])
                    reward[sPrime[0]]=self.mdp.getReward(s, stateAction, sPrime[0])





                IterValues.append((s_value+reward)*probs)
              if len(IterValues)!=0:
              
                self.values[s]=max(IterValues)

          


 
          index=index+1
     








    def getValue(self, state):
        """
          Return the value of the state (computed in __init__).
        """
        return self.values[state]


    def computeQValueFromValues(self, state, action):
        """
          Compute the Q-value of action in state from the
          value function stored in self.values.
        """
        "*** YOUR CODE HERE ***"
        probs=util.Counter()
        s_value=util.Counter()
        reward=util.Counter()
        StatesAndProbs=self.mdp.getTransitionStatesAndProbs(state,action)
        for states in StatesAndProbs:
          probs[states[0]]=states[1]
          s_value[states[0]]=self.discount*(self.values[states[0]])
          reward[states[0]]=self.mdp.getReward(state,action,states[0])
        
        
        return (s_value+reward)*probs


        

    def computeActionFromValues(self, state):
        """
          The policy is the best action in the given state
          according to the values currently stored in self.values.

          You may break ties any way you see fit.  Note that if
          there are no legal actions, which is the case at the
          terminal state, you should return None.
        """
        "*** YOUR CODE HERE ***"
        actionCounter=util.Counter()
        possibleActions=self.mdp.getPossibleActions(state)
        for action in possibleActions:
          actionCounter[action]=self.computeQValueFromValues(state,action)
        return actionCounter.argMax()




        

    def getPolicy(self, state):
        return self.computeActionFromValues(state)

    def getAction(self, state):
        "Returns the policy at the state (no exploration)."
        return self.computeActionFromValues(state)

    def getQValue(self, state, action):
        return self.computeQValueFromValues(state, action)
