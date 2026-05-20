import { useState, useEffect, useMemo } from 'react';
import { useAuth } from '../../context/AuthContext';
import { createEscrow, getUserEscrows } from '../../api/escrow.api';
import EscrowCard from '../../components/EscrowCard';
import styles from './dashboard.module.css';

export default function DashboardPage() {
  const { token, userId, role } = useAuth();
  const [escrows, setEscrows] = useState([]);
  const [formData, setFormData] = useState({ seller_id: '', amount: '' });
  const [loading, setLoading] = useState(false);
  const [message, setMessage] = useState('');
  const [error, setError] = useState('');

  const handleInputChange = (e) => {
    const { name, value } = e.target;
    setFormData((prev) => ({ ...prev, [name]: value }));
  };

  const handleCreateEscrow = async (e) => {
    e.preventDefault();
    setMessage('');
    setError('');

    if (!formData.seller_id || !formData.amount) {
      setError('Seller ID and amount are required');
      return;
    }

    setLoading(true);
    const result = await createEscrow(token, {
      seller_id: parseInt(formData.seller_id, 10),
      amount: parseFloat(formData.amount),
    });

    if (result.success) {
      const newEscrow = {
        escrow_id: result.data.data.escrow_id,
        status: result.data.data.status,
        seller_id: parseInt(formData.seller_id, 10),
        amount: parseFloat(formData.amount),
      };
      setEscrows((prev) => [
        {
          ...newEscrow,
          buyer_id: result.data.data.buyer_id,
          buyer_username: result.data.data.buyer_username,
          seller_username: result.data.data.seller_username,
        },
        ...prev,
      ]);
      setMessage('Escrow created successfully!');
      setFormData({ seller_id: '', amount: '' });
    } else {
      setError(result.error);
    }

    setLoading(false);
  };

  const handleStatusChange = (escrowId, newStatus) => {
    setEscrows((prev) => prev.map((e) => (e.escrow_id === escrowId ? { ...e, status: newStatus } : e)));
  };

  useEffect(() => {
    if (!token) return;
    let mounted = true;
    (async () => {
      const res = await getUserEscrows(token);
      if (mounted && res && res.success && res.data && res.data.data && res.data.data.escrows) {
        const items = res.data.data.escrows.map((e) => ({
          escrow_id: e.id,
          buyer_id: e.buyer_id,
          buyer_username: e.buyer_username,
          seller_id: e.seller_id,
          seller_username: e.seller_username,
          agent_id: e.agent_id,
          amount: e.amount,
          status: e.status,
        }));
        setEscrows(items);
      }
    })();
    return () => { mounted = false; };
  }, [token]);

  const visibleEscrows = useMemo(() => {
    if (!userId || !role) {
      return escrows;
    }

    if (role === 'SELLER') {
      return escrows.filter((escrow) => escrow.seller_id === userId);
    }

    if (role === 'BUYER') {
      return escrows.filter((escrow) => escrow.buyer_id === userId);
    }

    return escrows;
  }, [escrows, userId, role]);

  return (
    <div className={styles.container}>
      <h1 className={styles.pageTitle}>Escrow Dashboard</h1>

      <div className={styles.layout}>
        {/* Create Escrow Form */}
        <section className={styles.createSection}>
          <h2 className={styles.sectionTitle}>Create New Escrow</h2>
          <div className={styles.card}>
            {message && (
              <div className={styles.successAlert}>✓ {message}</div>
            )}
            {error && (
              <div className={styles.errorAlert}>✗ {error}</div>
            )}

            <form onSubmit={handleCreateEscrow} className={styles.form}>
              <div className={styles.formGroup}>
                <label htmlFor="seller_id" className={styles.label}>Seller ID</label>
                <input
                  type="number"
                  id="seller_id"
                  name="seller_id"
                  className={styles.input}
                  value={formData.seller_id}
                  onChange={handleInputChange}
                  required
                  disabled={loading}
                />
              </div>

              <div className={styles.formGroup}>
                <label htmlFor="amount" className={styles.label}>Amount (Ksh)</label>
                <input
                  type="number"
                  id="amount"
                  name="amount"
                  className={styles.input}
                  value={formData.amount}
                  onChange={handleInputChange}
                  step="0.01"
                  min="0"
                  required
                  disabled={loading}
                />
              </div>

              <button type="submit" className={styles.submitBtn} disabled={loading}>
                {loading ? 'Creating...' : 'Create Escrow'}
              </button>
            </form>
          </div>
        </section>

        {/* Escrow List */}
        <section className={styles.listSection}>
          <h2 className={styles.sectionTitle}>
            Active Escrows
            {visibleEscrows.length > 0 && <span className={styles.count}>{visibleEscrows.length}</span>}
          </h2>

          {visibleEscrows.length === 0 ? (
            <div className={styles.emptyState}><p>No escrows yet for this account.</p></div>
          ) : (
            <div className={styles.escrowList}>
              {visibleEscrows.map((escrow) => (
                <EscrowCard key={escrow.escrow_id} escrow={escrow} onStatusChange={handleStatusChange} />
              ))}
            </div>
          )}
        </section>
      </div>
    </div>
  );
}
