// Copyright (c) 2017 GeometryFactory Sarl (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Simon Giraudot

#ifndef CGAL_CLASSIFICATION_TRAINER_H
#define CGAL_CLASSIFICATION_TRAINER_H

#include <CGAL/Classification/Feature_base.h>
#include <CGAL/Classification/Label.h>
#include <CGAL/Classifier.h>

//#define CGAL_CLASSTRAINING_VERBOSE
#if defined(CGAL_CLASSTRAINING_VERBOSE)
#define CGAL_CLASSTRAINING_CERR std::cerr
#else
#define CGAL_CLASSTRAINING_CERR std::ostream(0)
#endif

namespace CGAL {

namespace Classification {

template <typename ItemRange, typename ItemMap>
class Trainer
{

public:
  typedef CGAL::Classifier<ItemRange, ItemMap>      Classifier;
  typedef typename Classification::Label_handle      Label_handle;
  typedef typename Classification::Feature_handle Feature_handle;

private:
  Classifier* m_classifier;
  std::vector<std::vector<std::size_t> > m_training_sets;

public:

  Trainer (Classifier& classifier)
    : m_classifier (&classifier)
    , m_training_sets (classifier.number_of_labels())
  {
  }

  /*!
    \brief Adds the item at position `index` as an inlier of
    `label` for the training algorithm.

    \note This inlier is only used for training. There is no guarantee
    that the item at position `index` will be classified as `label`
    after calling `run()`, `run_with_local_smoothing()` or
    `run_with_graphcut()`.

    \return `true` if the inlier was correctly added, `false`
    otherwise (if `label` was not found).
  */
  bool set_inlier (Label_handle label, std::size_t index)
  {
    std::size_t label_idx = (std::size_t)(-1);
    for (std::size_t i = 0; i < m_classifier->number_of_labels(); ++ i)
      if (m_classifier->label(i) == label)
        {
          label_idx = i;
          break;
        }
    if (label_idx == (std::size_t)(-1))
      return false;

    if (label_idx >= m_training_sets.size())
      m_training_sets.resize (label_idx + 1);

    m_training_sets[label_idx].push_back (index);

    return true;
  }

  /*!

    \brief Adds the items at positions `indices` as inliers of
    `label` for the training algorithm.

    \note These inliers are only used for training. There is no
    guarantee that the items at positions `indices` will be classified
    as `label` after calling `run()`,
    `run_with_local_smoothing()` or `run_with_graphcut()`.

    \tparam IndexRange range of `std::size_t`, model of `ConstRange`.
  */
  template <class IndexRange>
  bool set_inliers (Label_handle label,
                    IndexRange indices)
  {
    std::size_t label_idx = (std::size_t)(-1);
    for (std::size_t i = 0; i < m_classifier->number_of_labels(); ++ i)
      if (m_classifier->label(i) == label)
        {
          label_idx = i;
          break;
        }
    if (label_idx == (std::size_t)(-1))
      return false;

    if (label_idx >= m_training_sets.size())
      m_training_sets.resize (label_idx + 1);

    std::copy (indices.begin(), indices.end(), std::back_inserter (m_training_sets[label_idx]));

    return true;
  }
  
  /*!
    \brief Resets inlier sets used for training.
  */
  void reset_inlier_sets()
  {
    std::vector<std::vector<std::size_t > >().swap (m_training_sets);
  }

  /*!
    \brief Runs the training algorithm.

    All the `Classification::Label` and `Classification::Feature`
    necessary for classification should have been added before running
    this function. Each label must have ben given a small set
    of user-defined inliers to provide the training algorithm with a
    ground truth (see `set_inlier()` and `set_inliers()`).

    This methods estimates the set of feature weights and of
    [effects](@ref Classification::Feature::Effect) that make the
    classifier succeed in correctly classifying the sets of inliers
    given by the user. These parameters are directly modified within
    the `Classification::Feature_base` and `Classification::Label`
    objects. After training, the user can call `run()`,
    `run_with_local_smoothing()` or `run_with_graphcut()` to compute
    the classification using the estimated parameters.

    \param nb_tests number of tests to perform. Higher values may
    provide the user with better results at the cost of a higher
    computation time. Using a value of at least 10 times the number of
    features is advised.

    \return minimum ratio (over all labels) of provided
    ground truth items correctly classified using the best
    configuration found.
  */
  
  double train (std::size_t nb_tests = 300)
  {
    if (m_training_sets.empty())
      return 0.;

    for (std::size_t i = 0; i < m_classifier->number_of_labels(); ++ i)
      if (m_training_sets.size() <= i || m_training_sets[i].empty())
        std::cerr << "WARNING: \"" << m_classifier->label(i)->name() << "\" doesn't have a training set." << std::endl;

    std::vector<double> best_weights (m_classifier->number_of_features(), 1.);

    struct Feature_training
    {
      bool skipped;
      double wmin;
      double wmax;
      double factor;
    };
    std::vector<Feature_training> att_train;
    std::size_t nb_trials = 100;
    double wmin = 1e-5, wmax = 1e5;
    double factor = std::pow (wmax/wmin, 1. / (double)nb_trials);
    std::size_t att_used = 0;
    for (std::size_t j = 0; j < m_classifier->number_of_features(); ++ j)
      {
        Feature_handle att = m_classifier->feature(j);
        best_weights[j] = att->weight();

        std::size_t nb_useful = 0;
        double min = (std::numeric_limits<double>::max)();
        double max = -(std::numeric_limits<double>::max)();

        att->set_weight(wmin);
        for (std::size_t i = 0; i < 100; ++ i)
          {
            estimate_feature_effect(att);
            if (feature_useful(att))
              {
                CGAL_CLASSTRAINING_CERR << "#";
                nb_useful ++;
                min = (std::min) (min, att->weight());
                max = (std::max) (max, att->weight());
              }
            else
              CGAL_CLASSTRAINING_CERR << "-";
            att->set_weight(factor * att->weight());
          }
        CGAL_CLASSTRAINING_CERR << std::endl;
        CGAL_CLASSTRAINING_CERR << att->name() << " useful in "
                                << nb_useful << "% of the cases, in interval [ "
                                << min << " ; " << max << " ]" << std::endl;
        att_train.push_back (Feature_training());
        att_train.back().skipped = false;
        att_train.back().wmin = min / factor;
        att_train.back().wmax = max * factor;
        if (nb_useful < 2)
          {
            att_train.back().skipped = true;
            att->set_weight(0.);
            best_weights[j] = att->weight();
          }
        else if (best_weights[j] == 1.)
          {
            att->set_weight(0.5 * (att_train.back().wmin + att_train.back().wmax));
            best_weights[j] = att->weight();
            ++ att_used;
          }
        else
          {
            att->set_weight(best_weights[j]);
            ++ att_used;
          }
        estimate_feature_effect(att);
      }

    std::size_t nb_trials_per_feature = 1 + (std::size_t)(nb_tests / (double)(att_used));
    CGAL_CLASSIFICATION_CERR << "Trials = " << nb_tests << ", features = " << att_used
              << ", trials per att = " << nb_trials_per_feature << std::endl;
    for (std::size_t i = 0; i < att_train.size(); ++ i)
      if (!(att_train[i].skipped))
        att_train[i].factor = std::pow (att_train[i].wmax / att_train[i].wmin,
                                        1. / (double)nb_trials_per_feature);
    
    
    double best_score = compute_worst_score(0.);
    double best_confidence = compute_worst_confidence(0.);
    
    CGAL_CLASSIFICATION_CERR << "TRAINING GLOBALLY: Best score evolution: " << std::endl;

    CGAL_CLASSIFICATION_CERR << 100. * best_score << "% (found at initialization)" << std::endl;

    std::size_t current_att_changed = 0;
    for (std::size_t i = 0; i < att_used; ++ i)
      {
        while (att_train[current_att_changed].skipped)
          {
            ++ current_att_changed;
            if (current_att_changed == m_classifier->number_of_features())
              current_att_changed = 0;
          }

        std::size_t nb_used = 0;
        for (std::size_t j = 0; j < m_classifier->number_of_features(); ++ j)
          {
            if (j == current_att_changed)
              continue;
            
            m_classifier->feature(j)->set_weight(best_weights[j]);
            estimate_feature_effect(m_classifier->feature(j));
            if (feature_useful(m_classifier->feature(j)))
              nb_used ++;
          }
        Feature_handle current_att = m_classifier->feature(current_att_changed);
        const Feature_training& tr = att_train[current_att_changed];
        
        current_att->set_weight(tr.wmin);
        for (std::size_t j = 0; j < nb_trials_per_feature; ++ j)
          {
            estimate_feature_effect(current_att);

            double worst_confidence = compute_worst_confidence(best_confidence);

            double worst_score = compute_worst_score(best_score);

            if (worst_score > best_score
                && worst_confidence > best_confidence)
              {
                best_score = worst_score;
                best_confidence = worst_confidence;
                CGAL_CLASSIFICATION_CERR << 100. * best_score << "% (found at iteration "
                          << (i * nb_trials_per_feature) + j << "/" << nb_tests << ", "
                          << nb_used + (feature_useful(current_att) ? 1 : 0)
                          << "/" << m_classifier->number_of_features() << " feature(s) used)" << std::endl;
                for (std::size_t k = 0; k < m_classifier->number_of_features(); ++ k)
                  {
                    Feature_handle att = m_classifier->feature(k);
                    best_weights[k] = att->weight();
                  }
              }
            
            current_att->set_weight(current_att->weight() * tr.factor);
          }

        ++ current_att_changed;
      }

    for (std::size_t i = 0; i < best_weights.size(); ++ i)
      {
        Feature_handle att = m_classifier->feature(i);
        att->set_weight(best_weights[i]);
      }

    estimate_features_effects();
    
    CGAL_CLASSIFICATION_CERR << std::endl << "Best score found is at least " << 100. * best_score
              << "% of correct classification" << std::endl;

    std::size_t nb_removed = 0;
    for (std::size_t i = 0; i < best_weights.size(); ++ i)
      {
        Feature_handle att = m_classifier->feature(i);
        CGAL_CLASSTRAINING_CERR << "FEATURE " << att->name() << ": " << best_weights[i] << std::endl;
        att->set_weight(best_weights[i]);

        Classification::Feature::Effect side = m_classifier->label(0)->feature_effect(att);
        bool to_remove = true;
        for (std::size_t j = 0; j < m_classifier->number_of_labels(); ++ j)
          {
            Label_handle clabel = m_classifier->label(j);
            if (clabel->feature_effect(att) == Classification::Feature::FAVORING)
              CGAL_CLASSTRAINING_CERR << " * Favored for ";
            else if (clabel->feature_effect(att) == Classification::Feature::PENALIZING)
              CGAL_CLASSTRAINING_CERR << " * Penalized for ";
            else
              CGAL_CLASSTRAINING_CERR << " * Neutral for ";
            if (clabel->feature_effect(att) != side)
              to_remove = false;
            CGAL_CLASSTRAINING_CERR << clabel->name() << std::endl;
          }
        if (to_remove)
          {
            CGAL_CLASSTRAINING_CERR << "   -> Useless! Should be removed" << std::endl;
            ++ nb_removed;
          }
      }
    CGAL_CLASSIFICATION_CERR << nb_removed
              << " feature(s) out of " << m_classifier->number_of_features() << " are useless" << std::endl;
    
    return best_score;
  }
  

  /// @}

  
  /// \cond SKIP_IN_MANUAL
  Label_handle training_label_of (std::size_t index) const
  {
    for (std::size_t i = 0; i < m_training_sets.size(); ++ i)
      if (std::find (m_training_sets[i].begin(), m_training_sets[i].end(), index) != m_training_sets[i].end())
        return m_classifier->label(i);
    return Label_handle();
  }

  /// \endcond

private:

  void estimate_features_effects()
  {
    for (std::size_t i = 0; i < m_classifier->number_of_features(); ++ i)
      estimate_feature_effect (m_classifier->feature(i));
  }

  void estimate_feature_effect (Feature_handle att)
  {
    std::vector<double> mean (m_classifier->number_of_labels(), 0.);
                                  
    for (std::size_t j = 0; j < m_classifier->number_of_labels(); ++ j)
      {
        for (std::size_t k = 0; k < m_training_sets[j].size(); ++ k)
          {
            double val = att->normalized(m_training_sets[j][k]);
            mean[j] += val;
          }
        mean[j] /= m_training_sets[j].size();
      }

    std::vector<double> sd (m_classifier->number_of_labels(), 0.);
        
    for (std::size_t j = 0; j < m_classifier->number_of_labels(); ++ j)
      {
        Label_handle clabel = m_classifier->label(j);
            
        for (std::size_t k = 0; k < m_training_sets[j].size(); ++ k)
          {
            double val = att->normalized(m_training_sets[j][k]);
            sd[j] += (val - mean[j]) * (val - mean[j]);
          }
        sd[j] = std::sqrt (sd[j] / m_training_sets[j].size());
        if (mean[j] - sd[j] > 0.5)
          clabel->set_feature_effect (att, Classification::Feature::FAVORING);
        else if (mean[j] + sd[j] < 0.5)
          clabel->set_feature_effect (att, Classification::Feature::PENALIZING);
        else
          clabel->set_feature_effect (att, Classification::Feature::NEUTRAL);
      }
  }

  
  double compute_worst_score (double lower_bound)
  {
    double worst_score = 1.;
    for (std::size_t j = 0; j < m_classifier->number_of_labels(); ++ j)
      {
        std::size_t nb_okay = 0;
        for (std::size_t k = 0; k < m_training_sets[j].size(); ++ k)
          {
            std::size_t nb_class_best=0; 
            double val_class_best = (std::numeric_limits<double>::max)();
      
            for(std::size_t l = 0; l < m_classifier->number_of_labels(); ++ l)
              {
                double value = m_classifier->classification_value (m_classifier->label(l),
                                                                   m_training_sets[j][k]);
          
                if(val_class_best > value)
                  {
                    val_class_best = value;
                    nb_class_best = l;
                  }
              }
                
            if (nb_class_best == j)
              nb_okay ++;

          }

        double score = nb_okay / (double)(m_training_sets[j].size());
        if (score < worst_score)
          worst_score = score;
        if (worst_score < lower_bound)
          return worst_score;
      }
    return worst_score;
  }

  double compute_worst_confidence (double lower_bound)
  {
    double worst_confidence = (std::numeric_limits<double>::max)();
    for (std::size_t j = 0; j < m_classifier->number_of_labels(); ++ j)
      {
        double confidence = 0.;
        
        for (std::size_t k = 0; k < m_training_sets[j].size(); ++ k)
          {
            std::vector<std::pair<double, std::size_t> > values;
      
            for(std::size_t l = 0; l < m_classifier->number_of_labels(); ++ l)
              values.push_back (std::make_pair (m_classifier->classification_value (m_classifier->label(l),
                                                                                    m_training_sets[j][k]),
                                                l));
            std::sort (values.begin(), values.end());

            if (values[0].second == j)
              confidence += values[1].first - values[0].first;
            else
              {
                // for(std::size_t l = 0; l < values.size(); ++ l)
                //   if (values[l].second == j)
                //     {
                //       confidence += values[0].first - values[l].first;
                //       break;
                //     }
              }
            
          }

        confidence /= (double)(m_training_sets[j].size() * m_classifier->number_of_features());

        if (confidence < worst_confidence)
          worst_confidence = confidence;
        if (worst_confidence < lower_bound)
          return worst_confidence;
      }
    return worst_confidence;
  }

  bool feature_useful (Feature_handle att)
  {
    Classification::Feature::Effect side = m_classifier->label(0)->feature_effect(att);
    for (std::size_t k = 1; k < m_classifier->number_of_labels(); ++ k)
      if (m_classifier->label(k)->feature_effect(att) != side)
        return true;
    return false;
  }

  

};


} // namespace Classification
  
} // namespace CGAL

#endif // CGAL_CLASSIFICATION_TRAINER_H
