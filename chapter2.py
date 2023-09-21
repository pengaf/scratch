import numpy as np
import random

def greedy(qs):
    #ismaxs = qs.max() == qs
    #indices = [i for i in range(len(qs)) if ismaxs[i]]
    #return random.choice(indices)#slow
    return qs.argmax()

def epsilon_greedy(qs, epsilon):
    if random.random() < epsilon:
        return random.randint(0,len(qs)-1)
    else:
        return greedy(qs)

def ucb(qs, ns, t, c):
    tmp = qs + c * np.sqrt(np.log(t)/ns)
    return tmp.argmax()

def softmax(hs):
    es = np.exp(hs)
    es /= es.sum()
    return es

    