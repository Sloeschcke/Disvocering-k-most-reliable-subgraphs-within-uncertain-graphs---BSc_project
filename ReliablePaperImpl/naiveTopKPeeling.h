#ifndef NAIVETOPKPEELING_H
#define NAIVETOPKPEELING_H
#include "Hoved.h"
#include "Graph.h"
#include "Utility.h"
#include "Apriori.h"
#include "IterApriori.h"
#include "FastPeeling.h"
#include "TopKPeeling.h"
#include "MiningNonMaximal.h"

vector<Candidate> extractMax(vector<Candidate> tempRes, int k){
    sort(tempRes.begin(), tempRes.end());
    vector<Candidate> res = {};
    for(int i = 0; i<k; i++){
        res.push_back(tempRes[i]);
    }
    return res;
}

vector<Candidate> naiveTopKPeeling(vector<vector<vector<int>>>& graphSamples, vector<vector<int>>& filteredComponents, int numSamples, int k, Graph& uncertain){
    long double theta = 0.95;
    vector<Candidate> tempRes = {};
    while(tempRes.size() < k){
        set<set<int>> maximalFI = getMFI(filteredComponents, theta, numSamples);
        vector<Candidate> MFCS = fastPeeling(graphSamples, maximalFI, theta, numSamples);
        vector<vector<int>> MFCSNodes = extractNodes(MFCS);
        if(MFCSNodes.size() > 0){
            tempRes = runNonMaximal(graphSamples, uncertain, MFCSNodes, theta);
            tempRes.insert(tempRes.end(), MFCS.begin(), MFCS.end());
        } 
        theta = theta -0.1;
    }
    vector<Candidate> res = extractMax(tempRes, k);
    return res;
}

vector<Candidate> runNaiveTopKPeelingWithoutSampling(vector<vector<vector<int>>>& samples, int numSamples, int k, Graph graph){
    vector<vector<int>> components = connectedComponents(&samples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components,2);
    return naiveTopKPeeling(samples, filteredComponents, numSamples, k, graph);
}

vector<Candidate> runNaiveTopKPeeling(string fileName, int k, long double eps, long double delta){
    Graph graph (fileName);
    int numSamples = calculateRequiredSamples(eps, delta, graph.numNodes);
    vector<vector<vector<int>>> graphSamples = sample(graph, numSamples);
    return runNaiveTopKPeelingWithoutSampling(graphSamples, numSamples, k, graph);
}
#endif
