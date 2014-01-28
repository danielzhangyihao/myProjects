'''Unit tests for Ants Vs. SomeBees (ants.py).'''

import unittest
import doctest
import os
import sys
from ucb import main
import ants


class AntTest(unittest.TestCase):

    def setUp(self):
        hive, layout = ants.Hive(ants.make_test_assault_plan()), ants.test_layout
        self.colony = ants.AntColony(None, hive, ants.ant_types(), layout)


class TestProblem2(AntTest):

    def test_food_costs(self):
        error_msg = 'Wrong food_cost for ant class'
        self.assertEqual(2, ants.HarvesterAnt.food_cost, error_msg)
        self.assertEqual(4, ants.ThrowerAnt.food_cost, error_msg)

    def test_harvester(self):
        error_msg = 'HarvesterAnt did not add one food'
        old_food = self.colony.food
        ants.HarvesterAnt().action(self.colony)
        self.assertEqual(old_food + 1, self.colony.food, error_msg)


class TestProblem3(AntTest):

    def test_connectedness(self):
        error_msg = 'Entrances not properly initialized'
        for entrance in self.colony.bee_entrances:
            cur_place = entrance
            while cur_place:
                self.assertIsNotNone(cur_place.entrance, msg=error_msg)
                cur_place = cur_place.exit


class TestProblemB4(AntTest):

    def test_nearest_bee(self):
        error_msg = 'ThrowerAnt can\'t find the nearest bee.'
        ant = ants.ThrowerAnt()
        self.colony.places['tunnel_0_0'].add_insect(ant)
        near_bee = ants.Bee(2)
        self.colony.places['tunnel_0_3'].add_insect(near_bee)
        self.colony.places['tunnel_0_6'].add_insect(ants.Bee(2))
        hive = self.colony.hive
        self.assertIs(ant.nearest_bee(hive), near_bee, error_msg)
        ant.action(self.colony)
        self.assertEqual(1, near_bee.armor, error_msg)


    def test_nearest_bee_not_in_hive(self):
        error_msg = 'ThrowerAnt hit a Bee in the Hive'
        ant = ants.ThrowerAnt()
        self.colony.places['tunnel_0_0'].add_insect(ant)
        hive = self.colony.hive
        hive.add_insect(ants.Bee(2))
        self.assertIsNone(ant.nearest_bee(hive), error_msg)


class TestProblemB5(AntTest):

    def test_thrower_parameters(self):
        short_t = ants.ShortThrower()
        long_t = ants.LongThrower()
        self.assertEqual(3, ants.ShortThrower.food_cost, 'ShortThrower has wrong cost')
        self.assertEqual(3, ants.LongThrower.food_cost, 'LongThrower has wrong cost')
        self.assertEqual(1, short_t.armor, 'ShortThrower has wrong armor')
        self.assertEqual(1, long_t.armor, 'LongThrower has wrong armor')

    def test_inheritance(self):
        """Tests to see if the Long and Short Throwers are actually using the
        inherited action from the ThrowerAnt.
        """
        old_thrower_action = ants.ThrowerAnt.action
        old_throw_at = ants.ThrowerAnt.throw_at
        def new_thrower_action(self, colony):
            raise NotImplementedError()
        def new_throw_at(self, target):
            raise NotImplementedError()

        failed_long = 0
        try: # Test action
            ants.ThrowerAnt.action = new_thrower_action
            test_long = ants.LongThrower()
            test_long.action(self.colony)
        except NotImplementedError:
            failed_long += 1
        finally:
            ants.ThrowerAnt.action = old_thrower_action

        try: # Test throw_at
            ants.ThrowerAnt.throw_at = new_throw_at
            test_long = ants.LongThrower()
            test_bee = ants.Bee(1)
            test_long.throw_at(test_bee)
        except NotImplementedError:
            failed_long += 1
        finally:
            ants.ThrowerAnt.throw_at = old_throw_at

        if failed_long < 2:
            self.fail(msg="LongThrower is not using inheritance!")

        failed_short = 0
        try: # Test action
            ants.ThrowerAnt.action = new_thrower_action
            test_short = ants.ShortThrower()
            test_short.action(self.colony)
        except NotImplementedError:
            failed_short += 1
        finally:
            ants.ThrowerAnt.action = old_thrower_action

        try: # Test throw_at
            ants.ThrowerAnt.throw_at = new_throw_at
            test_short = ants.ShortThrower()
            test_bee = ants.Bee(1)
            test_short.throw_at(test_bee)
        except NotImplementedError:
            failed_short += 1
        finally:
            ants.ThrowerAnt.throw_at = old_throw_at

        if failed_short < 2:
            self.fail(msg="ShortThrower is not using inheritance!")

    def test_long(self):
        error_msg = 'LongThrower has the wrong range'
        ant = ants.LongThrower()
        self.colony.places['tunnel_0_0'].add_insect(ant)
        out_of_range, in_range = ants.Bee(2), ants.Bee(2)
        self.colony.places['tunnel_0_3'].add_insect(out_of_range)
        self.colony.places['tunnel_0_4'].add_insect(in_range)
        ant.action(self.colony)
        self.assertEqual(in_range.armor, 1, error_msg)
        self.assertEqual(out_of_range.armor, 2, error_msg)

    def test_short(self):
        error_msg = 'ShortThrower has the wrong range'
        ant = ants.ShortThrower()
        self.colony.places['tunnel_0_0'].add_insect(ant)
        out_of_range, in_range = ants.Bee(2), ants.Bee(2)
        self.colony.places['tunnel_0_3'].add_insect(out_of_range)
        self.colony.places['tunnel_0_2'].add_insect(in_range)
        ant.action(self.colony)
        self.assertEqual(in_range.armor, 1, error_msg)
        self.assertEqual(out_of_range.armor, 2, error_msg)




class TestProblemB6(AntTest):

    def test_scuba_parameters(self):
        scuba = ants.ScubaThrower()
        self.assertEqual(5, ants.ScubaThrower.food_cost, 'ScubaThrower has wrong cost')
        self.assertEqual(1, scuba.armor, 'ScubaThrower has wrong armor')

    def test_inheritance(self):
        """Tests to see if the ScubaThrower is actually using the inherited
        action from the ThrowerAnt.
        """
        old_thrower_action = ants.ThrowerAnt.action
        def new_thrower_action(self, colony):
            raise NotImplementedError()
        old_throw_at = ants.ThrowerAnt.throw_at
        def new_throw_at(self, target):
            raise NotImplementedError()

        failed_scuba = 0
        try:
            ants.ThrowerAnt.action = new_thrower_action
            test_scuba = ants.ScubaThrower()
            test_scuba.action(self.colony)
        except NotImplementedError:
            failed_scuba += 1
        finally:
            ants.ThrowerAnt.action = old_thrower_action

        try:
            ants.ThrowerAnt.throw_at = new_throw_at
            test_scuba = ants.ScubaThrower()
            test_bee = ants.Bee(1)
            test_scuba.throw_at(test_bee)
        except NotImplementedError:
            failed_scuba += 1
        finally:
            ants.ThrowerAnt.throw_at = old_throw_at

        if failed_scuba < 2:
            self.fail(msg="ScubaThrower is not using inheritance!")

    def test_scuba(self):
        error_msg = 'ScubaThrower sank'
        water = ants.Water('water')
        ant = ants.ScubaThrower()
        water.add_insect(ant)
        self.assertIs(water, ant.place, error_msg)
        self.assertEqual(1, ant.armor, error_msg)

    def test_scuba_on_land(self):
        place1 = self.colony.places['tunnel_0_0']
        place2 = self.colony.places['tunnel_0_4']
        ant = ants.ScubaThrower()
        bee = ants.Bee(3)
        place1.add_insect(ant)
        place2.add_insect(bee)
        ant.action(self.colony)
        self.assertEqual(2, bee.armor, 'ScubaThrower doesn\'t throw on land')


class TestProblemB7(AntTest):

    def test_hungry_parameters(self):
        hungry = ants.HungryAnt()
        self.assertEqual(4, ants.HungryAnt.food_cost, 'HungryAnt has wrong cost')
        self.assertEqual(1, hungry.armor, 'HungryAnt has wrong armor')

    def test_hungry_eats_and_digests(self):
        hungry = ants.HungryAnt()
        super_bee, super_pal = ants.Bee(1000), ants.Bee(1)
        place = self.colony.places['tunnel_0_0']
        place.add_insect(hungry)
        place.add_insect(super_bee)
        hungry.action(self.colony)
        self.assertEqual(0, super_bee.armor, 'HungryAnt didn\'t eat')
        place.add_insect(super_pal)
        for _ in range(3):
            hungry.action(self.colony)
        self.assertEqual(1, super_pal.armor, 'HungryAnt didn\'t digest')
        hungry.action(self.colony)
        self.assertEqual(0, super_pal.armor, 'HungryAnt didn\'t eat again')




@main
def main(*args):
    import argparse
    parser = argparse.ArgumentParser(description="Run Ants Tests")
    parser.add_argument('--verbose', '-v', action='store_true')
    args = parser.parse_args()
    doctest.testmod(ants, verbose=args.verbose)
    stdout = sys.stdout
    with open(os.devnull, 'w') as sys.stdout:
        verbosity = 2 if args.verbose else 1
        tests = unittest.main(exit=False, verbosity=verbosity)
    sys.stdout = stdout
