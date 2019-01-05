#include <string.h>
//#include <iostream>
#include "utils.h"
#include "spoa.hpp"
//#include "spoa_align.h"

extern "C" {
    int spoa_msa(char **seqs, int seq_n, char *cons_seq) {
        std::vector<std::string> sequences; //(seqs, seqs+seq_n);
        sequences.assign(seqs, seqs+seq_n);
        auto alignment_engine = spoa::createAlignmentEngine(static_cast<spoa::AlignmentType>(1), 5, -4, -8); 
        auto graph = spoa::createGraph();
        for (const auto & it: sequences) {
            auto alignment = alignment_engine->align_sequence_with_graph(it, graph);
            graph->add_alignment(alignment, it);
        }
        std::string consensus = graph->generate_consensus();
        strcpy(cons_seq, consensus.c_str());
        int cons_len = consensus.length();
#ifdef __DEBUG__
        std::vector<std::string> msa;
        graph->generate_multiple_sequence_alignment(msa);
        for (const auto& it: msa) {
            fprintf(stdout, "%s\n", it.c_str());
        }
#endif
        return cons_len;
    }
}

#ifdef _SPOA_ALIGN_MAIN

int main(void) {
    char *seqs[10] = {"AACGT", "AAGT", "AACCGT", "ACGT"};
    char *cons_seq = (char*)malloc(10);
    spoa_msa(seqs, 4, cons_seq);
    printf("%s\n", cons_seq);
    free(cons_seq);
    return 0;
}

#endif
